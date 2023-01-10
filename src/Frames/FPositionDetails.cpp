#include "Frames/FPositionDetails.h"

namespace XQuant {

	FPositionDetails::FPositionDetails(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FPositionDetails::onAttach() {

	}

	void FPositionDetails::onDetach() {

	}

	void FPositionDetails::onEvent(Event& e) {

	}

	void FPositionDetails::onImGuiRender() {
		ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_None);
		if (ImGui::SmallButton("[Debug] FPositionDetails"))
		{

		}
		ImGui::End();
	}
}

