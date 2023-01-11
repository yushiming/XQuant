
#include <imgui_internal.h>

#include "Frames/FMainMenuBar.h"
#include "Frames/FAccountLogin.h"

#include "Core/Application.h"


namespace XQuant {

	FMainMenuBar::FMainMenuBar(std::string name)
		: ImGuiFrame(name)
	{
	}

	void FMainMenuBar::onAttach() {

	}

	void FMainMenuBar::onDetach() {

	}

	void FMainMenuBar::onEvent(Event& e) {

	}

	void FMainMenuBar::onImGuiRender() {

		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu(u8"ϵͳ")) {
				if (ImGui::MenuItem(u8"�˺ŵ�¼")) {
					// ��¼��¼ 			
					// TOFIX ��ط����Ը�Ϊ�¼� ����½��浽������
					auto faccountlogin = new FAccountLogin(u8"�˺ŵ�¼");
					Application::instance().setAddVectorImGuiFrame(faccountlogin);
				}
				if (ImGui::MenuItem(u8"�˳�")) {

				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(u8"��ͼ")) {

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(u8"����")) {
				if (ImGui::MenuItem(u8"�༭�ز�")) {

				}
				//if (ImGui::MenuItem(u8"")) {

				//}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(u8"����")) {
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(u8"����")) {
				ImGui::EndMenu();
			}
			//_menuBarHeight = ImGui::GetCurrentWindow()->MenuBarHeight();
			ImGui::EndMainMenuBar();
		}
	}

	/*
				//IMGUI_DEMO_MARKER("Examples/Menu");
				ImGui::MenuItem("(demo menu)", NULL, false, false);
				if (ImGui::MenuItem("New")) {}

				if (ImGui::BeginMenu("Open Recent"))
				{
					ImGui::MenuItem("fish_hat.c");
					ImGui::MenuItem("fish_hat.inl");
					ImGui::MenuItem("fish_hat.h");
					if (ImGui::BeginMenu("More.."))
					{
						ImGui::MenuItem("Hello");
						ImGui::MenuItem("Sailor");
						if (ImGui::BeginMenu("Recurse.."))
						{
							//ShowExampleMenuFile();
							ImGui::EndMenu();
						}
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Save", "Ctrl+S")) {}
				if (ImGui::MenuItem("Save As..")) {}

				ImGui::Separator();
				//IMGUI_DEMO_MARKER("Examples/Menu/Options");
				if (ImGui::BeginMenu("Options"))
				{
					static bool enabled = true;
					ImGui::MenuItem("Enabled", "", &enabled);
					ImGui::BeginChild("child", ImVec2(0, 60), true);
					for (int i = 0; i < 10; i++)
						ImGui::Text("Scrolling Text %d", i);
					ImGui::EndChild();
					static float f = 0.5f;
					static int n = 0;
					ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
					ImGui::InputFloat("Input", &f, 0.1f);
					ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
	
	*/

}

