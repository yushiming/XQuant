//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  ProjectData.h
//
//   Description:  �������ݣ���������ƽ̨�˺���Ϣ��
//
//       Version:  1.0
//       Created:  2023��1��5�� 15ʱ23��14��
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

		std::string _projectName = "";				// ��������
		std::string _projectPath = "";				// ����·�� E:/tbc
		std::string _exportPath = "C:/";            // ����·��

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
