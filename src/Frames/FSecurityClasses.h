//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FSecurityClasses.h
//
//   Description:  ֤ȯ������
//
//       Version:  1.0
//       Created:  2023��1��8�� 14ʱ19��08��
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

	class FSecurityClasses : public ImGuiFrame
	{
	public:
		FSecurityClasses(std::string name);
		~FSecurityClasses() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
	};
}
