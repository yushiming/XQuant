#include "Frames/FSecurityClasses.h"

#include "Core/Config.h"
#include "Core/Application.h"

namespace XQuant {

	FSecurityClasses::FSecurityClasses(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FSecurityClasses::onAttach() {
		_isShow = true;
		_winSize = ImVec2(750.0f, 520.0f);
	}

	void FSecurityClasses::onDetach() {

	}

	void FSecurityClasses::onEvent(Event& e) {

	}

	void FSecurityClasses::onImGuiRender() {
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

		ImGui::SetCursorPos({ 50, 50 });
		ImGui::SetNextItemWidth(150);
		ImGui::BeginChild(u8"证券类别", {150, ImGui::GetWindowHeight() - 100});
		ImGui::Text(u8"证券类别");
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		static const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
		static int item_current_idx = 0; // Here we store our selection data as an index.
		// Custom size: use all width, 5 items tall
		if (ImGui::BeginListBox("##listbox 3", ImVec2(-FLT_MIN, ImGui::GetWindowHeight() - 110))) // 5 * ImGui::GetTextLineHeightWithSpacing()
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
		if (ImGui::SmallButton(u8"删除")) {

		}
		ImGui::PopID();

		ImGui::EndChild();

		// 类别品种
		ImGui::SetCursorPos({ 200, 50 });
		ImGui::SameLine();
		ImGui::SetNextItemWidth(150);
		ImGui::BeginChild(u8"类别子项", { 150, ImGui::GetWindowHeight() - 100 });
		ImGui::Text(u8"类别子项");
		ImGuiTabBarFlags tab_bar_flags1 = ImGuiTabBarFlags_None;

		// Custom size: use all width, 5 items tall
		if (ImGui::BeginListBox("##listbox 3", ImVec2(-FLT_MIN, ImGui::GetWindowHeight() - 110))) // 5 * ImGui::GetTextLineHeightWithSpacing()
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
		ImGui::PushID(2);
		if (ImGui::SmallButton(u8"删除")) {

		}
		ImGui::PopID();
		ImGui::EndChild();

		// 操作
		ImGui::SetCursorPos({ 350, 50 });
		ImGui::SameLine();
		ImGui::SetNextItemWidth(150);
		ImGui::BeginChild(u8"操作", { 150, ImGui::GetWindowHeight() - 100 });
		//ImGui::Text(u8"操作");
		//ImGui::PushID(2);
		ImGui::SetCursorPos({ 60.0f, ImGui::GetWindowHeight()*0.5f - 50 });
		if (ImGui::SmallButton(u8"新增")) {

		}
		//ImGui::PopID();

		ImGuiTabBarFlags tab_bar_flags2 = ImGuiTabBarFlags_None;
		
		ImGui::EndChild();

		// 全部品种
		ImGui::SetCursorPos({ 500, 50 });
		ImGui::SameLine();
		ImGui::SetNextItemWidth(150);
		ImGui::BeginChild(u8"全部品种", { 150, ImGui::GetWindowHeight() - 100 });
		ImGui::Text(u8"全部品种");
		ImGuiTabBarFlags tab_bar_flags3 = ImGuiTabBarFlags_None;
		// Custom size: use all width, 5 items tall
		if (ImGui::BeginListBox("##listbox 3", ImVec2(-FLT_MIN, ImGui::GetWindowHeight() - 110))) // 5 * ImGui::GetTextLineHeightWithSpacing()
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

		static char str0[128] = "1243365";
		if (ImGui::InputText(u8"代码", str0, IM_ARRAYSIZE(str0)))
		{
		}

		ImGui::EndChild();

		ImGui::End();
	}
}

