//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FLogOutput.h
//
//   Description:  日志输出界面
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
		bool _isShow = true;

	};
}
