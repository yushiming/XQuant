//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FStrategyClasses.h
//
//   Description:  ���������桢��������ѡ��
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

	class FStrategyClasses : public ImGuiFrame
	{
	public:
		FStrategyClasses(std::string name);
		~FStrategyClasses() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
	};
}
