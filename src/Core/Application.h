//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  Application.h
//
//   Description:  应用类
//
//       Version:  1.0
//       Created:  2023年1月2日 10时55分53秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================

#pragma once

#include "Core/Base.h"

#include "Core/Window.h"
#include "Core/FrameStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"

#include "Core/Timestep.h"

#include "ImGuiEXT/ImGuiLayer.h"
#include "ImGuiEXT/ImGuiFrame.h"

namespace XQuant {

	class Application
	{
	public:
		static Application& instance() { return *s_Instance; }

		Application(const std::string& name = "XQuant App");
		virtual ~Application();

		void onInit();
		void onEvent(Event& e);

		void pushLayer(ImGuiFrame* frame);
		void pushOverlay(ImGuiFrame* frame);

		Window& getWindow() { return *_window; }

		void close();

		ImGuiLayer* getImGuiLayer() { return _imGuiLayer; }

		void run();
	private:
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> _window;
		ImGuiLayer* _imGuiLayer = nullptr;
		bool _running = true;
		bool _minimized = false;
		FrameStack _frameStack;
		float _lastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();

}