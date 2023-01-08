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

#include <string>

#include "Core/Layer.h"
#include "Events/Event.h"

namespace XQuant {

	class ImGuiFrame : public Layer
	{
	public:
		ImGuiFrame(std::string name);
		~ImGuiFrame() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onEvent(Event& e) override;

		void begin();
		void end();

		void blockEvents(bool block) { _blockEvents = block; }

		void setDarkThemeColors();
	private:
		bool _blockEvents = true;
		float _time = 0.0f;
	};

}
