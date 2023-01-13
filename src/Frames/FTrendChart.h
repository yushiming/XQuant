//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  FTrendChart.h
//
//   Description:  ����ͼ���桢����K�����߻��ƣ����ߡ������ߡ���Ҫָ��
//
//       Version:  1.0
//       Created:  2023��1��6�� 11ʱ13��34��
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

	class FTrendChart : public ImGuiFrame
	{
	public:
		FTrendChart(std::string name);
		~FTrendChart() = default;

		void onAttach() override;
		void onDetach() override;
		void onEvent(Event& e) override;
		void onImGuiRender() override;

	private:
		void FTrendChart::drawCandlestickChart(const char* label_id, const double* xs, const double* opens, const double* closes, const double* lows, const double* highs, int count, bool tooltip, float width_percent, ImVec4 bullCol, ImVec4 bearCol);
	private:
	};
}
