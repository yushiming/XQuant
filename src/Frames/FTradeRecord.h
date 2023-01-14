//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FTradeRecord.h
//
//   Description:  ���׼�¼����
//
//       Version:  1.0
//       Created:  2023��1��6�� 12ʱ45��43��
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

	class FTradeRecord : public ImGuiFrame
	{
	public:
		FTradeRecord(std::string name);
		~FTradeRecord() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:

	};
}
