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
		if (!_isShow) {
			// TOFIX 这地方可以改为事件
			// Application::instance().setDeleteImGuiFrame(this);
			return;
		}

		if (_initWinPos) {
			// ImGui::SetNextWindowPos({ float(Config::ScreenWidth - _winWidth) / 2, float(Config::ScreenHeigth - _winHeigth) / 2 });
			_initWinPos = false;
		}

		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_None);


		ImGui::End();
	}
}

