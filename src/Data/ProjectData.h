//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  ProjectData.h
//
//   Description:  工程数据，包含各类平台账号信息、
//
//       Version:  1.0
//       Created:  2023年1月5日 15时23分14秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================

#pragma once 

#include <thread>
#include <mutex>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

namespace XQuant {

	class ProjectData {

	private:
		static ProjectData* _instance;
		static std::once_flag _flag;

		std::string _version = "1.1.0";

		std::string _projectName = "";				// 工程名字
		std::string _projectPath = "";				// 工程路径 E:/tbc
		std::string _exportPath = "C:/";            // 导出路径

	public:
		static const std::string worldXmlFileName;

	private:
		ProjectData();
		ProjectData(const ProjectData&) = delete;
		~ProjectData();

	public:
		static ProjectData* instance();

		static void destroy();

	public:

		std::string projectPath() {
			return _projectPath;
		}

		std::string assetsPath() {
			return _projectPath + "/" + "Assets";
		}


	};
}
