//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FHelp.h
//
//   Description:  帮助界面
//
//       Version:  1.0
//       Created:  2023年1月6日 17时52分29秒
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

	class FHelp : public ImGuiFrame
	{
	public:
		FHelp(std::string name);
		~FHelp() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
	};
}
