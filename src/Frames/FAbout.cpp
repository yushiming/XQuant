#include "Frames/FAbout.h"

#include "Core/Config.h"
#include "Core/Application.h"

namespace XQuant {

	FAbout::FAbout(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FAbout::onAttach() {
		_isShow = true;
		_winSize = ImVec2(600.0f, 400.0f);
	}

	void FAbout::onDetach() {

	}

	void FAbout::onEvent(Event& e) {

	}

	void FAbout::onImGuiRender() {
		if (!_isShow) {
			// TOFIX ��ط����Ը�Ϊ�¼�
			Application::instance().setDeleteImGuiFrame(this);
			return;
		}

		if (_initWinPos) {
			ImGui::SetNextWindowPos({ float(Config::ScreenWidth - _winSize.x) / 2, float(Config::ScreenHeigth - _winSize.y) / 2 });
			_initWinPos = false;
		}

		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);

		//ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);
		//ImGui::SetNextWindowSize(ImVec2(_winWidth, _winHeigth));
		ImGui::SetWindowSize(_winSize);

		ImGui::SetCursorPos({ 100, 50 });
		ImGui::BeginGroup();
		//ImGui::text(u8"�༭��ʹ��˵��: ");
		ImGui::Text(u8"�������: ���������������");
		ImGui::Text(u8"����: yushiimng");
		ImGui::EndGroup();

		ImGui::End();
	}
}

