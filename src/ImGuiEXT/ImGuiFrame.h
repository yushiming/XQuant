//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  ImGuiFrame.h
//
//   Description:  ImGui封装界面,
//
//       Version:  1.0
//       Created:  2023年1月3日 19时32分45秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================

#pragma once

#include <imgui.h>
#include <string>

#include "Core/TimeStep.h"
#include "Events/Event.h"

namespace XQuant {

	class ImGuiFrame
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		ImGuiFrame(std::string name);
		virtual ~ImGuiFrame();

		virtual void onAttach();
		virtual void onDetach();
		virtual void onUpdate(TimeStep ts) { }
		virtual void onImGuiRender() { }
		virtual void onEvent(Event& e);

		void setEventCallback(const EventCallbackFn& callback) { _eventCallback = callback; }
		void blockEvents(bool block) { _blockEvents = block; }
		const std::string& getName()const { return _name; }

	protected:
		std::string _name;
		bool _isShow = true;
		bool _initWinPos = true;
		ImVec2 _winPos;
		ImVec2 _winSize;
	private:
		bool _blockEvents = true;
		float _time = 0.0f;
		EventCallbackFn _eventCallback;
	};

}
