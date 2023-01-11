//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FAccountLogin.h
//
//   Description:  账号登录界面 包含期货CTP、股票XTP、外汇、数字货币
//
//       Version:  1.0
//       Created:  2023年1月6日 19时20分39秒
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
#include "Core/Common.h"

namespace XQuant {

	class FAccountLogin : public ImGuiFrame
	{
	public:
		FAccountLogin(std::string name);
		~FAccountLogin() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
		void drawFuturesLoginInfo();
		void drawStocksLoginInfo();
		void drawForexLoginInfo();
		void drawDigitalCashLoginInfo();

	private:
		bool _isShow = true;
		bool _initWinPos = true;

		const float _winWidth = 600.0f;
		const float _winHeigth = 400.0f;

		EPlatform _curSelPlatform = EPlatform::eNull;
		EPlatform _curPlatform = EPlatform::eNull;
	};
}
