//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FLogOutput.h
//
//   Description:  ��־�������
//
//       Version:  1.0
//       Created:  2023��1��6�� 17ʱ29��48��
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

	class FLogOutput : public ImGuiFrame
	{
	public:
		FLogOutput(std::string name);
		~FLogOutput() = default;

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
