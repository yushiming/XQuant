//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FPositionDetails.h
//
//   Description:  �ֲ���ϸ���桢������ǰ�˻��ʽ���Ϣ�����������ʡ����������ʡ��ֲ�ӯ������λ��������������
//
//       Version:  1.0
//       Created:  2023��1��6�� 16ʱ17��52��
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

	class FPositionDetails : public ImGuiFrame
	{
	public:
		FPositionDetails(std::string name);
		~FPositionDetails() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
	};
}
