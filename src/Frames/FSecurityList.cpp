#include "Frames/FSecurityList.h"

namespace XQuant {

	FSecurityList::FSecurityList(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FSecurityList::onAttach() {

	}

	void FSecurityList::onDetach() {

	}

	void FSecurityList::onEvent(Event& e) {

	}

	void FSecurityList::onImGuiRender() {
		ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_None);
		if (ImGui::SmallButton("[Debug] FSecurityList"))
		{

		}
		ImGui::End();
	}
}

