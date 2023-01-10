//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FStrategyBlueprintEdit.h
//
//   Description:  ������ͼ�༭���桢�����⡢��Ϊ��
//
//       Version:  1.0
//       Created:  2023��1��6�� 15ʱ41��19��
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

	class FStrategyBlueprintEdit : public ImGuiFrame
	{
	public:
		FStrategyBlueprintEdit(std::string name);
		~FStrategyBlueprintEdit() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
		bool _isShow = true;
	};
}
