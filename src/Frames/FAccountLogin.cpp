#include "Frames/FAccountLogin.h"

namespace XQuant {

	FAccountLogin::FAccountLogin(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FAccountLogin::onAttach() {

	}

	void FAccountLogin::onDetach() {

	}

	void FAccountLogin::onEvent(Event& e) {

	}

	void FAccountLogin::onImGuiRender() {

		ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoResize);
		if (ImGui::SmallButton("[Debug] FAccountLogin"))
		{

		}
		ImGui::End();

	}
}

