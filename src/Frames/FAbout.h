//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FAbout.h
//
//   Description:  关于界面
//
//       Version:  1.0
//       Created:  2023年1月6日 10时52分21秒
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

	class FAbout : public ImGuiFrame
	{
	public:
		FAbout(std::string name);
		~FAbout() = default;

		void onAttach() override;
		void onDetach() override;

		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:

	};
}
