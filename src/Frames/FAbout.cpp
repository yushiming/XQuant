#include "Frames/FAbout.h"

namespace XQuant {

	FAbout::FAbout(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FAbout::onAttach() {
		_isShow = true;
	}

	void FAbout::onDetach() {

	}

	void FAbout::onEvent(Event& e) {

	}

	void FAbout::onImGuiRender() {
		//ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoTitleBar);
		if (ImGui::SmallButton("[Debug] FAbout"))
		{

		}
		ImGui::End();
	}
}

