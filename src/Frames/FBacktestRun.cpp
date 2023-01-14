#include "Frames/FBacktestRun.h"

namespace XQuant {

	FBacktestRun::FBacktestRun(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FBacktestRun::onAttach() {
		_isShow = true;
	}

	void FBacktestRun::onDetach() {

	}

	void FBacktestRun::onEvent(Event& e) {

	}

	void FBacktestRun::onImGuiRender() {
		//ImGui::SetNextWindowSize(ImVec2(500, 400));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoTitleBar);
		if (ImGui::SmallButton("[Debug] FBacktestRun"))
		{

		}
		ImGui::End();
	}
}

