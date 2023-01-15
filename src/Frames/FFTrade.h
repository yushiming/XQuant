//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FFTrade.h
//
//   Description:  交易父窗口
//
//       Version:  1.0
//       Created:  2023年1月15日 23时57分58秒
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

	class FFTrade : public ImGuiFrame
	{
	public:
		FFTrade(std::string name);
		~FFTrade() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
	};
}
