//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FTradeLog.h
//
//   Description:  交易日志输出界面
//
//       Version:  1.0
//       Created:  2023年1月6日 17时29分48秒
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

	class FTradeLog : public ImGuiFrame
	{
	public:
		FTradeLog(std::string name);
		~FTradeLog() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
		void clearLog();

	private:
		ImGuiTextFilter          _filter;
		bool                     _autoScroll;
		bool	                 _copyToClipboard;
		std::vector<std::string> _logItems;
		ImVec4					 _logItemColor;
		bool                     _logItemHasColor;

	};
}
