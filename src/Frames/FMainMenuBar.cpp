
#include <imgui_internal.h>

#include "Frames/FMainMenuBar.h"
#include "Frames/FAccountLogin.h"
#include "Frames/FSecurityClasses.h"
#include "Frames/FSetting.h"
#include "Frames/FHelp.h"
#include "Frames/FAbout.h"

#include "Core/Application.h"


namespace XQuant {

	FMainMenuBar::FMainMenuBar(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FMainMenuBar::onAttach() {
		_isShow = true;
	}

	void FMainMenuBar::onDetach() {

	}

	void FMainMenuBar::onEvent(Event& e) {

	}

	void FMainMenuBar::onImGuiRender() {

		ImGui::SetNextWindowSize({ 0, 30 });

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::Button(u8"�˺�"))
			{
				// ��¼��¼ 			
				// TOFIX ��ط����Ը�Ϊ�¼� ����½��浽������
				auto faccountlogin = new FAccountLogin(u8"�˺ŵ�¼");
				Application::instance().setAddVectorImGuiFrame(faccountlogin);
			}
			if (ImGui::Button(u8"����ģʽ"))
			{

			}
			if (ImGui::Button(u8"����ģʽ"))
			{

			}
			if (ImGui::Button(u8"Ʒ����"))
			{
				auto fsecurityclasses = new FSecurityClasses(u8"Ʒ����");
				Application::instance().setAddVectorImGuiFrame(fsecurityclasses);
			}
			if (ImGui::Button(u8"����"))
			{
				auto fsetting = new FSetting(u8"����");
				Application::instance().setAddVectorImGuiFrame(fsetting);
			}

			if (ImGui::Button(u8"����"))
			{
				auto fhelp = new FHelp(u8"����");
				Application::instance().setAddVectorImGuiFrame(fhelp);
			}

			if (ImGui::Button(u8"����"))
			{
				auto fabout = new FAbout(u8"����");
				Application::instance().setAddVectorImGuiFrame(fabout);
			}

			if (ImGui::Button(u8"�˳�"))
			{
				Application::instance().close();
			}


			//if (ImGui::BeginMenu(u8"ϵͳ")) {
			//	if (ImGui::MenuItem(u8"�˺ŵ�¼")) {
			//		// ��¼��¼ 			
			//		// TOFIX ��ط����Ը�Ϊ�¼� ����½��浽������
			//		auto faccountlogin = new FAccountLogin(u8"�˺ŵ�¼");
			//		Application::instance().setAddVectorImGuiFrame(faccountlogin);
			//	}
			//	if (ImGui::MenuItem(u8"�˳�")) {

			//	}

			//	ImGui::EndMenu();
			//}
			//if (ImGui::BeginMenu(u8"��ͼ")) {

			//	ImGui::EndMenu();
			//}
			//if (ImGui::BeginMenu(u8"����")) {
			//	if (ImGui::MenuItem(u8"�༭�ز�")) {

			//	}
			//	//if (ImGui::MenuItem(u8"")) {

			//	//}

			//	ImGui::EndMenu();
			//}
			//if (ImGui::BeginMenu(u8"����")) {
			//	ImGui::EndMenu();
			//}
			//if (ImGui::BeginMenu(u8"����")) {
			//	ImGui::EndMenu();
			//}
			//_menuBarHeight = ImGui::GetCurrentWindow()->MenuBarHeight();
			ImGui::EndMainMenuBar();
		}
	}
}

