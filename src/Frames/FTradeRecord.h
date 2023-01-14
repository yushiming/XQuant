//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FTradeRecord.h
//
//   Description:  交易记录界面
//
//       Version:  1.0
//       Created:  2023年1月6日 12时45分43秒
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

	class FTradeRecord : public ImGuiFrame
	{
	public:
		FTradeRecord(std::string name);
		~FTradeRecord() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:

	};
}
