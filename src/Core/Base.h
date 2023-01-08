//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  Base.h
//
//   Description:  基础宏及函数类
//
//       Version:  1.0
//       Created:  2023年1月1日 13时54分23秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================

#pragma once

#include <memory>

#ifdef XQ_DEBUG
	#define XQ_DEBUGBREAK() __debugbreak()
	#define XQ_ENABLE_ASSERTS
#else
	#define XQ_DEBUGBREAK()
#endif

#define XQ_EXPAND_MACRO(x) x
#define XQ_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define XQ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


namespace XQuant {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}


}












