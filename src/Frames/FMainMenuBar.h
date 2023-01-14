//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FMainMenuBar.h
//
//   Description:  �����ڲ˵���
//
//       Version:  1.0
//       Created:  2023��1��6�� 10ʱ41��05��
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

	class FMainMenuBar : public ImGuiFrame
	{
	public:
		FMainMenuBar(std::string name);
		~FMainMenuBar() = default;

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
