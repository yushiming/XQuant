//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FSecurityList.h
//
//   Description:  ֤ȯ�б���� ����A��(��Ʊ)����������������������������������ѡ
//
//       Version:  1.0
//       Created:  2023��1��6�� 16ʱ37��51��
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
