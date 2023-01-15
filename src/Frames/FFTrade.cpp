#include "Frames/FFTrade.h"

namespace XQuant {

	FFTrade::FFTrade(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FFTrade::onAttach() {
		_isShow = true;
	}

	void FFTrade::onDetach() {

	}

	void FFTrade::onEvent(Event& e) {

	}

	void FFTrade::onImGuiRender() {
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

