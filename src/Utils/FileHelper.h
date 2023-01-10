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
		static std::string getFullPath(const std::string path);

	};

}
