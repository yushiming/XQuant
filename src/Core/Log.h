//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  Log.h
//
//   Description:  日志封装
//
//       Version:  1.0
//       Created:  2023年1月2日 16时16分56秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================

#pragma once

#include "Core/Base.h"

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace XQuant {

	class Log {
	public:
		static void init();

		static Ref<spdlog::logger>& getCoreLogger() {
			return _coreLogger;
		}
		
		static Ref<spdlog::logger>& getClientLogger() {
			return _clientLogger;
		}

	private:
		static Ref<spdlog::logger> _coreLogger;
		static Ref<spdlog::logger> _clientLogger;
	};
}

// Core log macros
#define XQ_CORE_TRACE(...)    ::XQuant::Log::getCoreLogger()->trace(__VA_ARGS__)
#define XQ_CORE_INFO(...)     ::XQuant::Log::getCoreLogger()->info(__VA_ARGS__)
#define XQ_CORE_WARN(...)     ::XQuant::Log::getCoreLogger()->warn(__VA_ARGS__)
#define XQ_CORE_ERROR(...)    ::XQuant::Log::getCoreLogger()->error(__VA_ARGS__)
#define XQ_CORE_CRITICAL(...) ::XQuant::Log::getCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define XQ_TRACE(...)         ::XQuant::Log::getClientLogger()->trace(__VA_ARGS__)
#define XQ_INFO(...)          ::XQuant::Log::getClientLogger()->info(__VA_ARGS__)
#define XQ_WARN(...)          ::XQuant::Log::getClientLogger()->warn(__VA_ARGS__)
#define XQ_ERROR(...)         ::XQuant::Log::getClientLogger()->error(__VA_ARGS__)
#define XQ_CRITICAL(...)      ::XQuant::Log::getClientLogger()->critical(__VA_ARGS__)








