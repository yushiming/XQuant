//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FFBacktest.h
//
//   Description:  回测父窗口
//
//       Version:  1.0
//       Created:  2023年1月15日 23时56分19秒
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

	class FFBacktest : public ImGuiFrame
	{
	public:
		FFBacktest(std::string name);
		~FFBacktest() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
	};
}
