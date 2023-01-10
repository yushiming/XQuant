//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FSetting.h
//
//   Description:  设置界面
//
//       Version:  1.0
//       Created:  2023年1月6日 16时28分49秒
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

	class FSetting : public ImGuiFrame
	{
	public:
		FSetting(std::string name);
		~FSetting() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
		bool _isShow = true;
	};
}
