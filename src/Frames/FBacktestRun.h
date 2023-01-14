//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FBacktestRun.h
//
//   Description:  回测运行界面、包含策略选择及因子参数修改
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
