//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  ImGuiLayer.h
//
//   Description:  ImGui���Ʋ��װ
//
//       Version:  1.0
//       Created:  2023��1��3�� 20ʱ18��31��
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================

#pragma once

#include "Events/Event.h"

namespace XQuant {

	class ImGuiLayer {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		ImGuiLayer();
		virtual ~ImGuiLayer();

		void onInit();
		void onEvent(Event& e);

		void begin();
		void end();

		void setEventCallback(const EventCallbackFn& callback) { _eventCallback = callback; }

		void blockEvents(bool block) { _blockEvents = block; }

		void setDarkThemeColors();
	private:
		void shutdown();
	private:
		bool _blockEvents = true;
		float _time = 0.0f;

		EventCallbackFn _eventCallback;
	};

}
