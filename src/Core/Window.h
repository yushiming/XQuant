//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  Window.h
//
//   Description:  应用窗口类
//
//       Version:  1.0
//       Created:  2023年1月2日 10时24分30秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================

#pragma once

#include <sstream>
#include <GLFW/glfw3.h>

#include "Core/Base.h"
#include "Events/Event.h"


namespace XQuant {

	struct WindowProps {
		std::string title;
		uint32_t width;
		uint32_t height;

		WindowProps(const std::string& t = "XQuant",
			uint32_t w = 1600,
			uint32_t h = 900)
			: title(t), width(w), height(h) {

		}
	};

	// 基于Window的桌面系统的接口
	class Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		Window(const WindowProps& props);
		virtual ~Window();

		void onInit();
		void onUpdate();

		unsigned int getWidth() const { return _data.width; }
		unsigned int getHeight() const { return _data.height; }

		// Window 特性
		void setEventCallback(const EventCallbackFn& callback) { _data.eventCallback = callback; }
		void setVSync(bool enabled);
		bool isVSync() const;

		void* getNativeWindow() const { return _window; }
		static Scope<Window> create(const WindowProps& props = WindowProps());
	private:
		void shutdown();
		int setWindowIcon(GLFWwindow* window);

	private:
		GLFWwindow* _window;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn eventCallback;
		};

		WindowData _data;
	
	};




}


