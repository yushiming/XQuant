//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FileDialogs.h
//
//   Description:  windows 打开和保存文件对话框
//
//       Version:  1.0
//       Created:  2023年1月3日 18时25分45秒
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

	class FileDialogs {

	public:
		// These return empty strings if cancelled
		static std::optional<std::string> openFile(const char* filter);
		static std::optional<std::string> saveFile(const char* filter);
	};

}
