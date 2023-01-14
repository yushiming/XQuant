//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FBacktestRun.h
//
//   Description:  �ز����н��桢��������ѡ�����Ӳ����޸�
//
//       Version:  1.0
//       Created:  2023��1��6�� 17ʱ35��46��
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

	class FBacktestRun : public ImGuiFrame
	{
	public:
		FBacktestRun(std::string name);
		~FBacktestRun() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
	};
}
