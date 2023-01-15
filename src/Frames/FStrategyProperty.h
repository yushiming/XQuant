//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FStrategyProperty.h
//
//   Description:  �������Լ����Ӳ����޸�
//
//       Version:  1.0
//       Created:  2023��1��9�� 12ʱ05��39��
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

	class FStrategyProperty : public ImGuiFrame
	{
	public:
		FStrategyProperty(std::string name);
		~FStrategyProperty() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
	};
}
