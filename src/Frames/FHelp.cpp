#include "Frames/FHelp.h"

#include "Core/Config.h"
#include "Core/Application.h"

namespace XQuant {

	FHelp::FHelp(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FHelp::onAttach() {
		_isShow = true;
		_winSize = ImVec2(600.0f, 400.0f);
	}

	void FHelp::onDetach() {

	}

	void FHelp::onEvent(Event& e) {

	}

	void FHelp::onImGuiRender() {
		if (!_isShow) {
			// TOFIX 这地方可以改为事件
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
		ImGui::Text(u8"编辑器使用说明: ");
		//ImGui::text("工程:");
		ImGui::BulletText(u8"账号按钮: 登录或退出各个平台账号");
		ImGui::BulletText(u8"交易模式: 界面设置为和交易相关");
		ImGui::BulletText(u8"策略模式: 界面设置为和策略相关");
		ImGui::EndGroup();

		ImGui::End();
	}
}

