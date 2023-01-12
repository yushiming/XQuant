#include "Frames/FTrendChart.h"

namespace XQuant {

	FTrendChart::FTrendChart(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FTrendChart::onAttach() {

	}

	void FTrendChart::onDetach() {

	}

	void FTrendChart::onEvent(Event& e) {

	}

	void FTrendChart::onImGuiRender() {
		// ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_None);
		if (ImGui::SmallButton("[Debug] FTrendChart"))
		{

		}
		ImGui::End();
	}
}

