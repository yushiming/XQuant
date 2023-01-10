#include "Frames/FAccountLogin.h"

#include "Data/ProjectData.h"
#include "Core/Config.h"

namespace XQuant {

	FAccountLogin::FAccountLogin(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FAccountLogin::onAttach() {
		_curPlatform = _curSelPlatform = ProjectData::instance()->getCurPlatform();
	}

	void FAccountLogin::onDetach() {

	}

	void FAccountLogin::onEvent(Event& e) {

	}

	void FAccountLogin::onImGuiRender() {

		if (_initWinPos) {
			ImGui::SetNextWindowPos({ float(Config::ScreenWidth - _winWidth) / 2, float(Config::ScreenHeigth - _winHeigth) / 2 });
			_initWinPos = false;
		}	

		// ImGui::SetNextWindowSize(ImVec2(_winWidth, _winHeigth));
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);
		ImGui::SetWindowSize({_winWidth, _winHeigth });

		// 绘制中间分隔线
		ImGui::GetWindowDrawList()->AddLine({ImGui::GetWindowPos().x + 180.0f, ImGui::GetWindowPos().y + 30.0f }, 
			{ImGui::GetWindowPos().x + 180.0f, ImGui::GetWindowPos().y + 390.0f }, ImColor(111, 111, 111, 255));

		ImGui::SetCursorPos({ 60.0f, 30.0f });
		//ImGui::PushFont(Config::FontBig);
		ImGui::Text(u8"交易平台");
		//ImGui::PopFont();

		// 绘制左边平台按钮
		// 期货
		ImGui::SetCursorPos({10.0f,65.0f});
		ImGui::PushStyleColor(ImGuiCol_Button, _curSelPlatform == EPlatform::eFutures ? Config::ImGuiColButton: ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		if (ImGui::Button(PlatformName[EPlatform::eFutures].c_str(), {150.0f, 40.0f }))
		{
			_curSelPlatform = EPlatform::eFutures;
		}
		ImGui::PopStyleColor();
		// 股票
		ImGui::SetCursorPos({ 10.0f,115.0f });
		ImGui::PushStyleColor(ImGuiCol_Button, _curSelPlatform == EPlatform::eStocks ? Config::ImGuiColButton : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		if (ImGui::Button(PlatformName[EPlatform::eStocks].c_str(), { 150.0f, 40.0f }))
		{
			_curSelPlatform = EPlatform::eStocks;
		}
		ImGui::PopStyleColor();
		// 外汇
		ImGui::SetCursorPos({ 10.0f,165.0f });
		ImGui::PushStyleColor(ImGuiCol_Button, _curSelPlatform == EPlatform::eForex ? Config::ImGuiColButton : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		if (ImGui::Button(PlatformName[EPlatform::eForex].c_str(), { 150.0f, 40.0f }))
		{
			_curSelPlatform = EPlatform::eForex;
		}
		ImGui::PopStyleColor();
		// 数字货币
		ImGui::SetCursorPos({ 10.0f,215.0f });
		ImGui::PushStyleColor(ImGuiCol_Button, _curSelPlatform == EPlatform::eDigitalCash ? Config::ImGuiColButton : ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
		if (ImGui::Button(PlatformName[EPlatform::eDigitalCash].c_str(), { 150.0f, 40.0f }))
		{
			_curSelPlatform = EPlatform::eDigitalCash;
		}
		ImGui::PopStyleColor();

		ImGui::SetCursorPos({ 190.0f,30.0f });
		ImGui::BeginChild(u8"LoginInfo", { 400.0f,360.0f }, true);

		switch (_curSelPlatform)
		{
		case EPlatform::eFutures:
			drawFuturesLoginInfo();
			break;
		case EPlatform::eStocks:
			drawStocksLoginInfo();
			break;
		case EPlatform::eForex:
			drawForexLoginInfo();
			break;
		case EPlatform::eDigitalCash:
			drawDigitalCashLoginInfo();
			break;
		default:
			break;
		}

		// 登录 退出
		ImGui::SetCursorPos({160.0f, 320.0f });
		ImGui::PushStyleColor(ImGuiCol_Button, _curSelPlatform == _curPlatform ? ImVec4(1.0f, 0.0f, 0.0f, 0.0f) : ImVec4(0.0f, 1.0f, 0.0f, 0.0f));
		if (ImGui::Button(_curSelPlatform == _curPlatform ? u8"退出" : u8"登录", {80.0f, 30.0f}))
		{
			_curSelPlatform == _curPlatform ? logout(_curSelPlatform) : login(_curSelPlatform);
		}
		ImGui::PopStyleColor();

		ImGui::EndChild();

		ImGui::End();
	}

	void FAccountLogin::drawFuturesLoginInfo() {
		ImGui::SetCursorPos({ 190.0f, 30.0f });
		ImGui::Text(u8"暂无该平台登录");

	}

	void FAccountLogin::drawStocksLoginInfo() {
		ImGui::SetCursorPos({ 190.0f, 30.0f });
		ImGui::Text(u8"暂无该平台登录");
	}

	void FAccountLogin::drawForexLoginInfo() {
		ImGui::SetCursorPos({ 190.0f, 30.0f });
		ImGui::Text(u8"暂无该平台登录");
	}

	void FAccountLogin::drawDigitalCashLoginInfo() {
		ImGui::SetCursorPos({ 190.0f, 30.0f });
		ImGui::Text(u8"暂无该平台登录");
	}

	void FAccountLogin::login(EPlatform platform) {
		// auto projectData = ProjectData::instance();
	}

	void FAccountLogin::logout(EPlatform platform) {

	}

}

