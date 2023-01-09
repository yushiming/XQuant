//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FTest.h
//
//   Description:  测试界面
//
//       Version:  1.0
//       Created:  2023年1月7日 18时04分52秒
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

	class FTest : public ImGuiFrame
	{
	public:
		FTest(std::string name);
		~FTest() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	};
}
