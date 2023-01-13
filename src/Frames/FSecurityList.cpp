#include "Frames/FSecurityList.h"
#include "Core/Application.h"

namespace XQuant {

	FSecurityList::FSecurityList(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FSecurityList::onAttach() {
		_isShow = true;
	}

	void FSecurityList::onDetach() {

	}

	void FSecurityList::onEvent(Event& e) {

	}

	void FSecurityList::onImGuiRender() {
		if (!_isShow) {
			// TOFIX 这地方可以改为事件
			Application::instance().setDeleteImGuiFrame(this);
			return;
		}

		if (_initWinPos) {
			//ImGui::SetNextWindowPos({ float(Config::ScreenWidth - _winSize.x) / 2, float(Config::ScreenHeigth - _winSize.y) / 2 });
			_initWinPos = false;
		}

		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_None);

		// ImGui::SetNextWindowSize(ImVec2(500, 400));
		// ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_None);

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("SecurityListTabBar", tab_bar_flags))
		{
			if (ImGui::BeginTabItem(u8"自选池"))
			{
				static const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIII", "JJJJ", "KKKK", "LLLLLLL", "MMMM", "OOOOOOO" };
				static int item_current_idx = 0; // Here we store our selection data as an index.

				// Custom size: use all width, 5 items tall
				if (ImGui::BeginListBox("##listbox 2", ImVec2(-FLT_MIN, -FLT_MIN))) // 5 * ImGui::GetTextLineHeightWithSpacing()
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

				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem(u8"买入"))
			{
				ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem(u8"卖出"))
			{
				ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem(u8"止损"))
			{
				ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::End();
	}
}

