//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FBacktestRecord.h
//
//   Description:  �ز��¼���� �������������ϸ��¼��������
//
//       Version:  1.0
//       Created:  2023��1��6�� 18ʱ42��30��
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

	class FBacktestRecord : public ImGuiFrame
	{
	public:
		FBacktestRecord(std::string name);
		~FBacktestRecord() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
		bool _isShow = true;
	};
}
