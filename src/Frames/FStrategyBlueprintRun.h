//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FStrategyBlueprintRun.h
//
//   Description:  ������ͼ���н���
//
//       Version:  1.0
//       Created:  2023��1��6�� 14ʱ23��16��
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

	class FStrategyBlueprintRun : public ImGuiFrame
	{
	public:
		FStrategyBlueprintRun(std::string name);
		~FStrategyBlueprintRun() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
	};
}
