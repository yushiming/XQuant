//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FAccountLogin.h
//
//   Description:  �˺ŵ�¼���� �����ڻ�CTP����ƱXTP����㡢���ֻ���
//
//       Version:  1.0
//       Created:  2023��1��6�� 19ʱ20��39��
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
	};
}
