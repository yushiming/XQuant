//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FFTrade.h
//
//   Description:  ���׸�����
//
//       Version:  1.0
//       Created:  2023��1��15�� 23ʱ57��58��
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
