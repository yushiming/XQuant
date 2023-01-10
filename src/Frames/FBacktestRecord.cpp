#include "Frames/FBacktestRecord.h"

namespace XQuant {

	FBacktestRecord::FBacktestRecord(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FBacktestRecord::onAttach() {

	}

	void FBacktestRecord::onDetach() {

	}

	void FBacktestRecord::onEvent(Event& e) {

	}

	void FBacktestRecord::onImGuiRender() {
		//ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoTitleBar);
		if (ImGui::SmallButton("[Debug] FBacktestRecord"))
		{

		}
		ImGui::End();
	}
}

