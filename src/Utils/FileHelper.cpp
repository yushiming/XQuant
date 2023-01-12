#include <regex>

#include <windows.h>
#include <sys/types.h>  
#include <sys/stat.h>  

#include <commdlg.h>
#include <optional>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Core/Application.h"
#include "Utils/FileHelper.h"
#include "Core/Assert.h"
#include "Core/Log.h"
#include "Utils/StringHelper.h"

namespace XQuant {
	std::string FileHelper::getAppPath() {
		char szFilePath[MAX_PATH + 1] = { 0 };
		GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
		(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串  
		std::string path = szFilePath;

		return path;
	}

	std::string FileHelper::getFullPath(const std::string& path) {
		return getAppPath() + "\\" + path;
	}

	bool FileHelper::isFileExist(const std::string& fileFullPath)
	{
		DWORD attr = GetFileAttributesW(StringHelper::utf8ToWideChar(fileFullPath).c_str());
		if (attr == INVALID_FILE_ATTRIBUTES || (attr & FILE_ATTRIBUTE_DIRECTORY))
			return false;   //  not a file
		return true;
	}


	bool FileHelper::isDirectoryExist(const std::string& dirPath)
	{
		unsigned long fAttrib = GetFileAttributes(StringHelper::utf8ToWideChar(dirPath).c_str());
		if (fAttrib != INVALID_FILE_ATTRIBUTES &&
			(fAttrib & FILE_ATTRIBUTE_DIRECTORY))
		{
			return true;
		}
		return false;
	}

	long FileHelper::getFileSize(const std::string& filepath)
	{
		struct _stat tmp;
		if (_stat(filepath.c_str(), &tmp) == 0)
		{
			return (long)tmp.st_size;
		}
		return 0;
	}

	bool FileHelper::renameFile(const std::string& oldfullpath, const std::string& newfullpath)
	{
		XQ_CORE_ASSERT(!oldfullpath.empty(), "Invalid path");
		XQ_CORE_ASSERT(!newfullpath.empty(), "Invalid path");

		std::wstring _wNew = StringHelper::utf8ToWideChar(newfullpath);
		std::wstring _wOld = StringHelper::utf8ToWideChar(oldfullpath);

		if (FileHelper::isFileExist(newfullpath))
		{
			if (!DeleteFile(_wNew.c_str()))
			{
				XQ_CORE_ERROR("Fail to delete file %s !Error code is 0x%x", newfullpath.c_str(), GetLastError());
			}
		}

		if (MoveFile(_wOld.c_str(), _wNew.c_str()))
		{
			return true;
		}
		else
		{
			XQ_CORE_ERROR("Fail to rename file %s to %s !Error code is 0x%x", oldfullpath.c_str(), newfullpath.c_str(), GetLastError());
			return false;
		}
	}

	bool FileHelper::renameFile(const std::string& path, const std::string& oldname, const std::string& name)
	{
		XQ_CORE_ASSERT(!path.empty(), "Invalid path");
		std::string oldPath = path + oldname;
		std::string newPath = path + name;

		std::regex pat("\\/");
		std::string _old = std::regex_replace(oldPath, pat, "\\");
		std::string _new = std::regex_replace(newPath, pat, "\\");

		return FileHelper::renameFile(_old, _new);
	}

	bool FileHelper::createDirectory(const std::string& dirPath)
	{
		XQ_CORE_ASSERT(!dirPath.empty(), "Invalid path");

		if (FileHelper::isDirectoryExist(dirPath))
			return true;

		std::wstring path = StringHelper::utf8ToWideChar(dirPath);

		// Split the path
		size_t start = 0;
		size_t found = path.find_first_of(L"/\\", start);
		std::wstring subpath;
		std::vector<std::wstring> dirs;

		if (found != std::wstring::npos)
		{
			while (true)
			{
				subpath = path.substr(start, found - start + 1);
				if (!subpath.empty())
					dirs.push_back(subpath);
				start = found + 1;
				found = path.find_first_of(L"/\\", start);
				if (found == std::wstring::npos)
				{
					if (start < path.length())
					{
						dirs.push_back(path.substr(start));
					}
					break;
				}
			}
		}

		if ((GetFileAttributes(path.c_str())) == INVALID_FILE_ATTRIBUTES)
		{
			subpath = L"";
			for (unsigned int i = 0, size = (unsigned int)dirs.size(); i < size; ++i)
			{
				subpath += dirs[i];

				std::string utf8Path = StringHelper::wideCharToUtf8(subpath);
				if (!FileHelper::isDirectoryExist(utf8Path))
				{
					BOOL ret = CreateDirectory(subpath.c_str(), NULL);
					if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
					{
						XQ_CORE_ERROR("Fail create directory %s !Error code is 0x%x", utf8Path.c_str(), GetLastError());
						return false;
					}
				}
			}
		}
		return true;
	}

	bool FileHelper::removeFile(const std::string& filepath)
	{
		std::regex pat("\\/");
		std::string win32path = std::regex_replace(filepath, pat, "\\");

		if (DeleteFile(StringHelper::utf8ToWideChar(win32path).c_str()))
		{
			return true;
		}
		else
		{
			XQ_CORE_ERROR("Fail remove file %s !Error code is 0x%x", filepath.c_str(), GetLastError());
			return false;
		}
	}

	bool FileHelper::removeDirectory(const std::string& dirPath)
	{
		std::string dirPathCopy = dirPath;
		if (dirPath.length() > 0 && dirPath[dirPath.length() - 1] != '/' && dirPath[dirPath.length() - 1] != '\\')
		{
			dirPathCopy.append("/");
		}
		std::wstring wpath = StringHelper::utf8ToWideChar(dirPathCopy);
		std::wstring files = wpath + L"*.*";
		WIN32_FIND_DATA wfd;
		HANDLE  search = FindFirstFileEx(files.c_str(), FindExInfoStandard, &wfd, FindExSearchNameMatch, NULL, 0);
		bool ret = true;
		if (search != INVALID_HANDLE_VALUE)
		{
			BOOL find = true;
			while (find)
			{
				// Need check string . and .. for delete folders and files begin name.
				std::wstring fileName = wfd.cFileName;
				if (fileName != L"." && fileName != L"..")
				{
					std::wstring temp = wpath + wfd.cFileName;
					if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						temp += '/';
						ret = ret && FileHelper::removeDirectory(StringHelper::wideCharToUtf8(temp));
					}
					else
					{
						SetFileAttributes(temp.c_str(), FILE_ATTRIBUTE_NORMAL);
						ret = ret && DeleteFile(temp.c_str());
					}
				}
				find = FindNextFile(search, &wfd);
			}
			FindClose(search);
		}
		if (ret && RemoveDirectory(wpath.c_str()))
		{
			return true;
		}
		return false;
	}

	std::string FileHelper::convertPathFormatToUnixStyle(const std::string& path)
	{
		std::string ret = path;
		int len = (int)ret.length();
		for (int i = 0; i < len; ++i)
		{
			if (ret[i] == '\\')
			{
				ret[i] = '/';
			}
		}
		return ret;
	}

}
