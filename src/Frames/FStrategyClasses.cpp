#include "Frames/FStrategyClasses.h"

namespace XQuant {

	FStrategyClasses::FStrategyClasses(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FStrategyClasses::onAttach() {
		_isShow = true;
	}

	void FStrategyClasses::onDetach() {

	}

	void FStrategyClasses::onEvent(Event& e) {

	}

	void FStrategyClasses::onImGuiRender() {

		if (!_isShow) {
			// TOFIX 这地方可以改为事件
			// Application::instance().setDeleteImGuiFrame(this);
			return;
		}

		if (_initWinPos) {
			// ImGui::SetNextWindowPos({ float(Config::ScreenWidth - _winSize.x) / 2, float(Config::ScreenHeigth - _winSize.y) / 2 });
			_initWinPos = false;
		}

		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_None);
		//ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove);
		//ImGui::SetNextWindowSize(ImVec2(_winWidth, _winHeigth));
		//ImGui::SetWindowSize(_winSize);
		// 策略类别
		ImGui::BeginChild(u8"策略类别", { 0.0f, ImGui::GetWindowHeight()*0.5f - 10 });
		ImGui::Text(u8"策略类别");
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		static const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
		static int item_current_idx = 0; // Here we store our selection data as an index.
		// Custom size: use all width, 5 items tall
		if (ImGui::BeginListBox("##listbox 3", ImVec2(-FLT_MIN, ImGui::GetWindowHeight() - 50))) // 5 * ImGui::GetTextLineHeightWithSpacing()
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				const bool is_selected = (item_current_idx == n);
				if (ImGui::Selectable(items[n], is_selected))
					item_current_idx = n;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndListBox();
		}

		ImGui::PushID(1);
		if (ImGui::SmallButton(u8"新增")) {

		}
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::PushID(2);
		if (ImGui::SmallButton(u8"删除")) {

		}
		ImGui::PopID();
		ImGui::EndChild();

		// 分割线
		//ImGui::GetWindowDrawList()->AddLine({ ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y*0.5f },
		//	{ ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowSize().y * 0.5f }, ImColor(111, 111, 111, 255));

		// 策略运行操作
		//ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.5f + 10);
		ImGui::BeginChild(u8"策略执行", { 0.0f, ImGui::GetWindowHeight() * 0.5f - 40 }, true);
		ImGui::Text(u8"当前策略: %s", u8"缠论二买");

		static int selected_tread2 = 0;
		const char* names2[] = { u8"品种集合", u8"单一品种" };
		// Simple selection popup (if you want to show the current selection inside the Button itself,
		// you may want to build a string using the "###" operator to preserve a constant ID with a variable label)
		ImGui::SetNextItemWidth(60);
		if (ImGui::Button(u8"测试类型"))
			ImGui::OpenPopup("my_select_popup2");
		ImGui::SameLine();
		ImGui::SetNextItemWidth(60);
		ImGui::TextUnformatted(names2[selected_tread2]);
		if (ImGui::BeginPopup("my_select_popup2"))
		{
			if (ImGui::Selectable(names2[0]))
				selected_tread2 = 0;
			if (ImGui::Selectable(names2[1]))
				selected_tread2 = 1;
			//for (int i = 0; i < IM_ARRAYSIZE(names); i++)
			//	if (ImGui::Selectable(names[i]))
			//		selected_tread1 = i;
			ImGui::EndPopup();
		}

		if (selected_tread2 == 1) {
			static char str0[128] = "22344";
			ImGui::InputText("代码", str0, IM_ARRAYSIZE(str0));
		}
		else if (selected_tread2 == 0)
		{
			const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIIIIII", "JJJJ", "KKKKKKK" };
			static int item_current = 0;
			ImGui::SetNextItemWidth(160);
			ImGui::Combo(u8"选择品种集", &item_current, items, IM_ARRAYSIZE(items));
		}

		ImGui::EndChild();

		ImGui::End();
	}
}

