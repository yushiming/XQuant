#include "Frames/FHelp.h"

namespace XQuant {

	FHelp::FHelp(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FHelp::onAttach() {
		_isShow = true;
	}

	void FHelp::onDetach() {

	}

	void FHelp::onEvent(Event& e) {

	}

	void FHelp::onImGuiRender() {
		//ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoTitleBar);
		if (ImGui::SmallButton("[Debug] FHelp"))
		{

		}
		ImGui::End();
	}
}

