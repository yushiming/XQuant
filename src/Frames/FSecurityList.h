//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FSecurityList.h
//
//   Description:  证券列表界面 沪深A股(股票)、满足条件、今日新增、今日卖出、自选
//
//       Version:  1.0
//       Created:  2023年1月6日 16时37分51秒
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

	class FSecurityList : public ImGuiFrame
	{
	public:
		FSecurityList(std::string name);
		~FSecurityList() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
		bool _isShow = true;
	};
}
