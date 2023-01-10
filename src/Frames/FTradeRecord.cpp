#include "Frames/FTradeRecord.h"

namespace XQuant {

	FTradeRecord::FTradeRecord(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FTradeRecord::onAttach() {

	}

	void FTradeRecord::onDetach() {

	}

	void FTradeRecord::onEvent(Event& e) {

	}

	void FTradeRecord::onImGuiRender() {
		ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_None);
		if (ImGui::SmallButton("[Debug] FTradeRecord"))
		{

		}
		ImGui::End();
	}
}

