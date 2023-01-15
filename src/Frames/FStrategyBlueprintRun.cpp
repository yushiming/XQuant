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
			// TOFIX ��ط����Ը�Ϊ�¼�
			// Application::instance().setDeleteImGuiFrame(this);
			return;
		}

		if (_initWinPos) {
			// ImGui::SetNextWindowPos({ float(Config::ScreenWidth - _winWidth) / 2, float(Config::ScreenHeigth - _winHeigth) / 2 });
			_initWinPos = false;
		}

		// �ֲ���ϸ���桢������ǰ�˻��ʽ���Ϣ�����������ʡ����������ʡ��ֲ�ӯ������λ��������������
		ImGui::Begin(_name.c_str(), &_isShow, ImGuiWindowFlags_NoTitleBar);

		ImGui::Text(u8"��ǰ����: %s", u8"���۶���");
		ImGui::SameLine();
		ImGuiHelper::helpMarker(u8"��������");

		const char* items[] = { "AAAA", "BBBB", "CCCC", "DDDD", "EEEE", "FFFF", "GGGG", "HHHH", "IIIIIII", "JJJJ", "KKKKKKK" };
		static int item_current = 0;
		ImGui::SetNextItemWidth(160);
		ImGui::Combo(u8"ѡ�����", &item_current, items, IM_ARRAYSIZE(items));
		ImGui::SameLine();
		ImGuiHelper::helpMarker(u8"��������");
		ImGui::SameLine();
		if (ImGui::Button(u8"����")) {

		}

		// �ָ���
		ImGui::GetWindowDrawList()->AddLine({ ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 80.0f },
			{ ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + 80.0f }, ImColor(111, 111, 111, 255));

		ImGui::SetCursorPosY(130.0f);



		ImGui::End();
	}
}

