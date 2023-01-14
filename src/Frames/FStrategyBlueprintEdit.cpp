#include "Frames/FStrategyBlueprintEdit.h"

namespace XQuant {

	FStrategyBlueprintEdit::FStrategyBlueprintEdit(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FStrategyBlueprintEdit::onAttach() {
		_isShow = true;
	}

	void FStrategyBlueprintEdit::onDetach() {

	}

	void FStrategyBlueprintEdit::onEvent(Event& e) {

	}

	void FStrategyBlueprintEdit::onImGuiRender() {
		//ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoTitleBar);
		if (ImGui::SmallButton("[Debug] FStrategyBlueprintEdit"))
		{

		}
		ImGui::End();
	}
}

