//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FMainToolBar.h
//
//   Description:  �����ڹ�����
//
//       Version:  1.0
//       Created:  2023��1��6�� 10ʱ49��28��
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

	class FMainToolBar : public ImGuiFrame
	{
	public:
		FMainToolBar(std::string name);
		~FMainToolBar() = default;

		void onAttach() override;
		void onDetach() override;

		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:

	private:
		bool _isShow = true;
		const float _toolbarSize = 50;
		float _menuBarHeight = 0;

	};
}
