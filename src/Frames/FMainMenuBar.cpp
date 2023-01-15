
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
			if (ImGui::Button(u8"账号"))
			{
				// 登录记录 			
				// TOFIX 这地方可以改为事件 添加新界面到窗口中
				auto faccountlogin = new FAccountLogin(u8"账号登录");
				Application::instance().setAddVectorImGuiFrame(faccountlogin);
			}
			if (ImGui::Button(u8"交易模式"))
			{

			}
			if (ImGui::Button(u8"策略模式"))
			{

			}
			if (ImGui::Button(u8"品种类"))
			{
				auto fsecurityclasses = new FSecurityClasses(u8"品种类");
				Application::instance().setAddVectorImGuiFrame(fsecurityclasses);
			}
			if (ImGui::Button(u8"设置"))
			{
				auto fsetting = new FSetting(u8"设置");
				Application::instance().setAddVectorImGuiFrame(fsetting);
			}

			if (ImGui::Button(u8"帮助"))
			{
				auto fhelp = new FHelp(u8"帮助");
				Application::instance().setAddVectorImGuiFrame(fhelp);
			}

			if (ImGui::Button(u8"关于"))
			{
				auto fabout = new FAbout(u8"关于");
				Application::instance().setAddVectorImGuiFrame(fabout);
			}

			if (ImGui::Button(u8"退出"))
			{
				Application::instance().close();
			}


			//if (ImGui::BeginMenu(u8"系统")) {
			//	if (ImGui::MenuItem(u8"账号登录")) {
			//		// 登录记录 			
			//		// TOFIX 这地方可以改为事件 添加新界面到窗口中
			//		auto faccountlogin = new FAccountLogin(u8"账号登录");
			//		Application::instance().setAddVectorImGuiFrame(faccountlogin);
			//	}
			//	if (ImGui::MenuItem(u8"退出")) {

			//	}

			//	ImGui::EndMenu();
			//}
			//if (ImGui::BeginMenu(u8"视图")) {

			//	ImGui::EndMenu();
			//}
			//if (ImGui::BeginMenu(u8"策略")) {
			//	if (ImGui::MenuItem(u8"编辑回测")) {

			//	}
			//	//if (ImGui::MenuItem(u8"")) {

			//	//}

			//	ImGui::EndMenu();
			//}
			//if (ImGui::BeginMenu(u8"设置")) {
			//	ImGui::EndMenu();
			//}
			//if (ImGui::BeginMenu(u8"帮助")) {
			//	ImGui::EndMenu();
			//}
			//_menuBarHeight = ImGui::GetCurrentWindow()->MenuBarHeight();
			ImGui::EndMainMenuBar();
		}
	}
}

