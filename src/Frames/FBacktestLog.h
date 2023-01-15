//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FBacktestLog.h
//
//   Description:  回测日志输出界面
//
//       Version:  1.0
//       Created:  2023年1月6日 17时56分08秒
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

	class FBacktestLog : public ImGuiFrame
	{
	public:
		FBacktestLog(std::string name);
		~FBacktestLog() = default;

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
