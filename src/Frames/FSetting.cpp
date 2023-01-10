#include "Frames/FSetting.h"

namespace XQuant {

	FSetting::FSetting(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FSetting::onAttach() {

	}

	void FSetting::onDetach() {

	}

	void FSetting::onEvent(Event& e) {

	}

	void FSetting::onImGuiRender() {
		//ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoTitleBar);
		if (ImGui::SmallButton("[Debug] FSetting"))
		{

		}
		ImGui::End();
	}
}

