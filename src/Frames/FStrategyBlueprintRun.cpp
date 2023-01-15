#include "Utils/ImGuiHelper.h"
#include "Frames/FStrategyBlueprintRun.h"


namespace XQuant {

	FStrategyBlueprintRun::FStrategyBlueprintRun(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FStrategyBlueprintRun::onAttach() {
		_isShow = true;
	}

	void FStrategyBlueprintRun::onDetach() {

	}

	void FStrategyBlueprintRun::onEvent(Event& e) {

	}

	void FStrategyBlueprintRun::onImGuiRender() {
		if (!_isShow) {
			// TOFIX 这地方可以改为事件
			// Application::instance().setDeleteImGuiFrame(this);
			return;
		}

		if (_initWinPos) {
			// ImGui::SetNextWindowPos({ float(Config::ScreenWidth - _winWidth) / 2, float(Config::ScreenHeigth - _winHeigth) / 2 });
			_initWinPos = false;
		}

		// 持仓明细界面、包含当前账户资金信息、总体收益率、今日收益率、持仓盈亏、仓位，交易买卖窗口
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoTitleBar);

		ImGui::Text(u8"当前策略: %s", u8"缠论二买");
		ImGui::SameLine();
		ImGuiHelper::helpMarker(u8"策略描述");

		const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIIIIII", "JJJJ", "KKKKKKK" };
		static int item_current = 0;
		ImGui::SetNextItemWidth(160);
		ImGui::Combo(u8"选择策略", &item_current, items, IM_ARRAYSIZE(items));
		ImGui::SameLine();
		ImGuiHelper::helpMarker(u8"策略描述");
		ImGui::SameLine();
		if (ImGui::Button(u8"启动")) {

		}

		// 分割线
		ImGui::GetWindowDrawList()->AddLine({ ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 80.0f },
			{ ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + 80.0f }, ImColor(111, 111, 111, 255));

		ImGui::SetCursorPosY(130.0f);



		ImGui::End();
	}
}

