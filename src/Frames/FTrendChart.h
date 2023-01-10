//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FTrendChart.h
//
//   Description:  走势图界面、包含K线曲线绘制，均线、布林线、主要指标
//
//       Version:  1.0
//       Created:  2023年1月6日 11时13分34秒
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

	class FTrendChart : public ImGuiFrame
	{
	public:
		FTrendChart(std::string name);
		~FTrendChart() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
		bool _isShow = true;
	};
}
