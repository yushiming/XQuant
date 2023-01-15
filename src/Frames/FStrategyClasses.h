//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FStrategyClasses.h
//
//   Description:  策略类别界面、包含策略选择
//
//       Version:  1.0
//       Created:  2023年1月6日 17时35分46秒
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
