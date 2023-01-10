#include "Frames/FMainToolBar.h"

#include <imgui_internal.h>

namespace XQuant {

	FMainToolBar::FMainToolBar(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FMainToolBar::onAttach() {

	}

	void FMainToolBar::onDetach() {

	}

	void FMainToolBar::onEvent(Event& e) {

	}

	void FMainToolBar::onImGuiRender() {
		//ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoTitleBar);
		if (ImGui::SmallButton("[Debug] FMainToolBar"))
		{

		}
		ImGui::End();
	}


}

