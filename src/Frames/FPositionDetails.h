//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FPositionDetails.h
//
//   Description:  持仓明细界面、包含当前账户资金信息、总体收益率、今日收益率、持仓盈亏、仓位，交易买卖窗口
//
//       Version:  1.0
//       Created:  2023年1月6日 16时17分52秒
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

	class FPositionDetails : public ImGuiFrame
	{
	public:
		FPositionDetails(std::string name);
		~FPositionDetails() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
	};
}
