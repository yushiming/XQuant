#include "Frames/FSetting.h"

#include "Core/Config.h"
#include "Core/Application.h"

namespace XQuant {

	FSetting::FSetting(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FSetting::onAttach() {
		_isShow = true;
		_winSize = ImVec2(600.0f, 400.0f);
	}

	void FSetting::onDetach() {

	}

	void FSetting::onEvent(Event& e) {

	}

	void FSetting::onImGuiRender() {
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
		ImGui::Text(u8"����: �������׻ز����");
		ImGui::Text(u8"����: ");
		ImGui::InputTextMultiline("##source", u8"�����ֶ����ס��������ס����Ա༭�ز���һ��������", 500, { 0, ImGui::GetTextLineHeight() * 10 }, ImGuiInputTextFlags_AllowTabInput | ImGuiInputTextFlags_CtrlEnterForNewLine);
		ImGui::EndGroup();

		ImGui::End();
	}
}

