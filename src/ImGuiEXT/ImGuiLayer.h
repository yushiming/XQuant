//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  ImGuiLayer.h
//
//   Description:  ImGui绘制层封装
//
//       Version:  1.0
//       Created:  2023年1月3日 20时18分31秒
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
		ImGuiLayer();
		virtual ~ImGuiLayer();

		void onInit();
		void onEvent(Event& e);

		void begin();
		void end();

		void blockEvents(bool block) { _blockEvents = block; }

		void setDarkThemeColors();
	private:
		void shutdown();
	private:
		bool _blockEvents = true;
		float _time = 0.0f;
	};

}
