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
#include <map>
#include <functional>

#include "Core/Common.h"
#include "Events/Event.h"

namespace XQuant {

	class ProjectData {

	private:
		ProjectData();
		ProjectData(const ProjectData&) = delete;
		~ProjectData();

	public:
		using EventCallbackFn = std::function<void(Event&)>;

		static ProjectData* instance();

		static void destroy();

		void onInit();
		void onEvent(Event& e);

		// 登录
		void login(EPlatform platform);

		// 退出
		void logout(EPlatform platform);


		// 账号配置数据读写
		bool readAccountConfig();
		void writerAccountConfig();

		bool readFuturesAccountJsonFile(std::string filename);
		void writerFuturesAccountJsonFile();

		bool readStocksAccountJsonFile(std::string filename);
		void writerStocksAccountJsonFile();

		bool readForexAccountJsonFile(std::string filename);
		void writerForexAccountJsonFile();

		bool readDigitalCashAccountJsonFile(std::string filename);
		void writerDigitalCashAccountJsonFile();


		std::string appPath() {
			return _appPath;
		}

		std::string configPath() {
			return _configPath;
		}

		std::string resourcesPath() {
			return _resourcesPath;
		}

		std::string tempPath() {
			return _tempPath;
		}

		EPlatform getCurPlatform() {
			return _platform;
		}

		bool isCurPlatform(EPlatform platform) {
			return _platform == platform;
		}

		bool setCurPlatform(EPlatform platform) {
			_platform = platform;
		}

		std::string getConfigFullPathFile(EPlatform platform);

		void setEventCallback(const EventCallbackFn& callback) { _eventCallback = callback; }

	public:
		static const std::string ConfigJsonFileName;

		static FuturesAccountInfo* futuresAccountInfo;
		static StocksAccountInfo* stocksAccountInfo;
		static ForexAccountInfo* forexAccountInfo;
		static DigitalCashAccountInfo* digitalcashAccountInfo;
		
	private:
		static ProjectData* _instance;
		static std::once_flag _flag;

		std::string _version = "1.1.0";

		std::string _projectName = "";				// 工程名字
		std::string _appPath = "";				    // 应用程序路径
		std::string _configPath = "";				// 配置文件路径
		std::string _resourcesPath = "";			// 资源文件路径
		std::string _tempPath = "";			        // 缓存文件路径

		EPlatform _platform = EPlatform::eNull;

		EventCallbackFn _eventCallback;

		std::map<EPlatform, std::string> _platformConfigPath;

	};
}
