#include "Frames/FMainStatusBar.h"

#include <imgui_internal.h>

namespace XQuant {

	FMainStatusBar::FMainStatusBar(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FMainStatusBar::onAttach() {

	}

	void FMainStatusBar::onDetach() {

	}

	void FMainStatusBar::onEvent(Event& e) {

	}

	void FMainStatusBar::onImGuiRender() {
		//ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoTitleBar);
		if (ImGui::SmallButton("[Debug] FMainStatusBar"))
		{

		}
		ImGui::End();
	}

}

