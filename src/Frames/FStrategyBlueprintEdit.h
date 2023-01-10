//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FStrategyBlueprintEdit.h
//
//   Description:  策略蓝图编辑界面、条件库、行为库
//
//       Version:  1.0
//       Created:  2023年1月6日 15时41分19秒
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
