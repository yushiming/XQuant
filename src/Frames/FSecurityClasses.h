//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FSecurityClasses.h
//
//   Description:  证券类别界面
//
//       Version:  1.0
//       Created:  2023年1月8日 14时19分08秒
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
