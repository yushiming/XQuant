#include "Frames/FBacktestLog.h"
#include "Core/Application.h"
#include "Utils/TimeHelper.h"

namespace XQuant {

	FBacktestLog::FBacktestLog(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FBacktestLog::onAttach() {
		_isShow = true;
		_logItemColor = ImVec4(0.8f, 0.2f, 0.2f, 1.0f);

		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " buy " + u8"����Ƽ�100��, �۸�12.5, �ܼ�12500Ԫ");
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " sell " + u8"������������100��, �۸�15.5, ӯ��1550Ԫ"); //
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " zhisun " + u8"ֹ���Ϻ�����100��, �۸�35.5, ����1200Ԫ"); //
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " buy " + u8"����Ƽ�100��, �۸�12.5, �ܼ�12500Ԫ");
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " sell " + u8"������������100��, �۸�15.5, ӯ��1550Ԫ"); //
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " zhisun " + u8"ֹ���Ϻ�����100��, �۸�35.5, ����1200Ԫ"); //
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " buy " + u8"����Ƽ�100��, �۸�12.5, �ܼ�12500Ԫ");
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " sell " + u8"������������100��, �۸�15.5, ӯ��1550Ԫ"); //
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " zhisun " + u8"ֹ���Ϻ�����100��, �۸�35.5, ����1200Ԫ"); //
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " buy " + u8"����Ƽ�100��, �۸�12.5, �ܼ�12500Ԫ");
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " sell " + u8"������������100��, �۸�15.5, ӯ��1550Ԫ"); //
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " zhisun " + u8"ֹ���Ϻ�����100��, �۸�35.5, ����1200Ԫ"); //
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " buy " + u8"����Ƽ�100��, �۸�12.5, �ܼ�12500Ԫ");
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " sell " + u8"������������100��, �۸�15.5, ӯ��1550Ԫ"); //
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " zhisun " + u8"ֹ���Ϻ�����100��, �۸�35.5, ����1200Ԫ"); //
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " buy " + u8"����Ƽ�100��, �۸�12.5, �ܼ�12500Ԫ");
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " sell " + u8"������������100��, �۸�15.5, ӯ��1550Ԫ"); //
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " zhisun " + u8"ֹ���Ϻ�����100��, �۸�35.5, ����1200Ԫ"); //
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " buy " + u8"����Ƽ�100��, �۸�12.5, �ܼ�12500Ԫ");
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " sell " + u8"������������100��, �۸�15.5, ӯ��1550Ԫ"); //
		_logItems.push_back(TimeHelper::getCurrentTimeStr() + " zhisun " + u8"ֹ���Ϻ�����100��, �۸�35.5, ����1200Ԫ"); //
	}

	void FBacktestLog::onDetach() {

	}

	void FBacktestLog::onEvent(Event& e) {

	}

	void FBacktestLog::onImGuiRender() {
		if (!_isShow) {
			// TOFIX ��ط����Ը�Ϊ�¼�
			//Application::instance().setDeleteImGuiFrame(this);
			return;
		}

		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_None);

		if (ImGui::SmallButton("Clear")) {
			clearLog(); 
		}
		ImGui::SameLine();
		_copyToClipboard = ImGui::SmallButton("Copy");

		// Options menu
		if (ImGui::BeginPopup("Options"))
		{
			ImGui::Checkbox("Auto-scroll", &_autoScroll);
			ImGui::EndPopup();
		}

		ImGui::SameLine();
		// Options, Filter
		if (ImGui::Button("Options"))
			ImGui::OpenPopup("Options");

		ImGui::SameLine();
		_filter.Draw("Filter", 180);
		ImGui::Separator();

		if (ImGui::BeginChild("LogScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
			if (_copyToClipboard)
				ImGui::LogToClipboard();
			for (int i = 0; i < (int)_logItems.size(); i++)
			{
				const char* item = _logItems[i].c_str();
				if (!_filter.PassFilter(item))
					continue;

				// Normally you would store more information in your item than just a string.
				// (e.g. make Items[] an array of structure, store color/type etc.)
				//if (strstr(item, "[buy]")) { _logItemColor = ImVec4(0.2f, 0.2f, 1.0f, 1.0f); _logItemHasColor = true; }
				//else if (strstr(item, "[sell]")) { _logItemColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); _logItemHasColor = true; }
				//else if (strstr(item, "[zhisun]")) { _logItemColor = ImVec4(1.0f, 0.2f, 0.2f, 1.0f); _logItemHasColor = true; }
				//else { _logItemColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); }

				//if (_logItemHasColor)
				ImGui::PushStyleColor(ImGuiCol_Text, _logItemColor);
				ImGui::TextUnformatted(item);
				//if (_logItemHasColor)
				ImGui::PopStyleColor();
			}
			if (_copyToClipboard)
				ImGui::LogFinish();

			// Keep up at the bottom of the scroll region if we were already at the bottom at the beginning of the frame.
			// Using a scrollbar or mouse-wheel will take away from the bottom edge.
			if (_autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
				ImGui::SetScrollHereY(1.0f);
			ImGui::PopStyleVar();
		}
		ImGui::EndChild();

		ImGui::End();
	}

	void FBacktestLog::clearLog() {
		_logItems.clear();
	}
}

