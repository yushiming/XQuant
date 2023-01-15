//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FStrategyProperty.h
//
//   Description:  策略属性及因子参数修改
//
//       Version:  1.0
//       Created:  2023年1月9日 12时05分39秒
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
