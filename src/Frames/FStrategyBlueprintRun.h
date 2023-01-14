//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FStrategyBlueprintRun.h
//
//   Description:  策略蓝图运行界面
//
//       Version:  1.0
//       Created:  2023年1月6日 14时23分16秒
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
