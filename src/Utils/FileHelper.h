//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FileHelper.h
//
//   Description:  文件及目录操作
//
//       Version:  1.0
//       Created:  2023年1月7日 11时25分05秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================

#pragma once

#include <string>
#include <optional>

namespace XQuant {

	class FileHelper {

	public:
		static std::string getAppPath();

		static std::string getFullPath(const std::string& path);

        static bool isFileExist(const std::string& fileFullPath);

        /**
        *  Checks whether a directory exists without considering search paths and resolution orders.
        *  @param dirPath The directory (with absolute path) to look up for
        *  @return Returns true if the directory found at the given absolute path, otherwise returns false
        */
        static bool isDirectoryExist(const std::string& dirFullPath);

        /**
        *  Renames a file under the given directory.
        *
        *  @param path     The parent directory path of the file, it must be an absolute path.
        *  @param oldname  The current name of the file.
        *  @param name     The new name of the file.
        *  @return True if the file have been renamed successfully, false if not.
        */
        bool renameFile(const std::string& path, const std::string& oldname, const std::string& name);

        /**
        *  Renames a file under the given directory.
        *
        *  @param oldfullpath  The current path + name of the file.
        *  @param newfullpath  The new path + name of the file.
        *  @return True if the file have been renamed successfully, false if not.
        */
        static bool renameFile(const std::string& oldfullpath, const std::string& newfullpath);

        /**
        *  Removes a file.
        *
        *  @param filepath The full path of the file, it must be an absolute path.
        *  @return True if the file have been removed successfully, false if not.
        */
        static bool removeFile(const std::string& filepath);

        /**
        *  Creates a directory.
        *
        *  @param dirPath The path of the directory, it must be an absolute path.
        *  @return True if the directory have been created successfully, false if not.
        */
        static bool createDirectory(const std::string& dirPath);

        /**
        *  Removes a directory.
        *
        *  @param dirPath  The full path of the directory, it must be an absolute path.
        *  @return True if the directory have been removed successfully, false if not.
        */
        static bool removeDirectory(const std::string& dirPath);

        static long getFileSize(const std::string& filepath);

        // D:\aaa\bbb\ccc\ddd\abc.txt --> D:/aaa/bbb/ccc/ddd/abc.txt
        static std::string convertPathFormatToUnixStyle(const std::string& path);
	};

}
