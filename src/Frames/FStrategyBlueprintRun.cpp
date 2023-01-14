#include "Frames/FStrategyBlueprintRun.h"

namespace XQuant {

	FStrategyBlueprintRun::FStrategyBlueprintRun(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FStrategyBlueprintRun::onAttach() {
		_isShow = true;
	}

	void FStrategyBlueprintRun::onDetach() {

	}

	void FStrategyBlueprintRun::onEvent(Event& e) {

	}

	void FStrategyBlueprintRun::onImGuiRender() {
		//ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoTitleBar);
		if (ImGui::SmallButton("[Debug] FStrategyBlueprintRun"))
		{

		}
		ImGui::End();
	}
}

