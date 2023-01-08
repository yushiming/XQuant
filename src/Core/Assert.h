//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  Assert.h
//
//   Description:  断言类
//
//       Version:  1.0
//       Created:  2023年1月1日 21时32分54秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================


#pragma once

#include "Core/Base.h"
#include "Core/Log.h"
#include <filesystem>

#ifdef XQ_ENABLE_ASSERTS
	// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
	// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
	#define XQ_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { XQ##type##ERROR(msg, __VA_ARGS__); XQ_DEBUGBREAK(); } }
	#define XQ_INTERNAL_ASSERT_WITH_MSG(type, check, ...) XQ_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define XQ_INTERNAL_ASSERT_NO_MSG(type, check) XQ_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", XQ_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define XQ_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define XQ_INTERNAL_ASSERT_GET_MACRO(...) XQ_EXPAND_MACRO( XQ_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, XQ_INTERNAL_ASSERT_WITH_MSG, XQ_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define XQ_ASSERT(...) XQ_EXPAND_MACRO( XQ_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define XQ_CORE_ASSERT(...) XQ_EXPAND_MACRO( XQ_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
	#define XQ_ASSERT(...)
	#define XQ_CORE_ASSERT(...)
#endif


