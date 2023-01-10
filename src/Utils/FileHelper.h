//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FileHelper.h
//
//   Description:  �ļ���Ŀ¼����
//
//       Version:  1.0
//       Created:  2023��1��7�� 11ʱ25��05��
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
