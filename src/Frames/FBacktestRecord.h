//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FBacktestRecord.h
//
//   Description:  回测记录界面 多少买卖点击详细记录、收益率
//
//       Version:  1.0
//       Created:  2023年1月6日 18时42分30秒
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

	class FBacktestRecord : public ImGuiFrame
	{
	public:
		FBacktestRecord(std::string name);
		~FBacktestRecord() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
		bool _isShow = true;
	};
}
