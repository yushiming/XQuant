//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FMainStatusBar.h
//
//   Description:  主窗口状态栏
//
//       Version:  1.0
//       Created:  2023年1月6日 10时43分52秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================

#pragma once

#include <string>

#include "ImGuiEXT/ImGuiFrame.h"
#include "Events/Event.h"

namespace XQuant {

	class FMainStatusBar : public ImGuiFrame
	{
	public:
		FMainStatusBar(std::string name);
		~FMainStatusBar() = default;

		void onAttach() override;
		void onDetach() override;

		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:


	private:
		const float _toolbarSize = 50;
		float _menuBarHeight = 0;

	};
}
