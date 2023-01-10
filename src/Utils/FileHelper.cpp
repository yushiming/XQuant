
#include <windows.h>
#include <commdlg.h>
#include <optional>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Core/Application.h"
#include "Utils/FileHelper.h"

namespace XQuant {
	std::string FileHelper::getAppPath() {
		char szFilePath[MAX_PATH + 1] = { 0 };
		GetModuleFileNameA(NULL, szFilePath, MAX_PATH);
		(strrchr(szFilePath, '\\'))[0] = 0; // 删除文件名，只获得路径字串  
		std::string path = szFilePath;

		return path;
	}

	std::string FileHelper::getFullPath(const std::string path) {
		return getAppPath() + "\\" + path;
	}

}
