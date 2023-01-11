
#include <string>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <Windows.h>

#include "Core/Config.h"
#include "Core/Application.h"
#include "Events/Event.h"
#include "ImGuiEXT/ImGuiLayer.h"
#include "ImGuiEXT/Implot/implot.h"
#include "Utils/FileHelper.h"

namespace XQuant {

	ImGuiLayer::ImGuiLayer() {

	}

	ImGuiLayer::~ImGuiLayer() {
		shutdown();
	}

	void ImGuiLayer::onInit()
	{
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImPlot::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;
		io.ConfigFlags |= ImGuiCol_DockingEmptyBg;

		// 多个字体及字体大小会极大影响内存 fzkaiti.ttf
		Config::FontBig = io.Fonts->AddFontFromFileTTF(FileHelper::getFullPath("resources\\font\\fzkaiti.ttf").c_str(), 16.0f, NULL,
			io.Fonts->GetGlyphRangesChineseFull());

		//Config::FontMiddle = io.Fonts->AddFontFromFileTTF(FileHelper::getFullPath("resources\\font\\sysongti.ttf").c_str(), 20.0f, NULL,
		//	io.Fonts->GetGlyphRangesChineseFull());

		//Config::FontSmall = io.Fonts->AddFontFromFileTTF(FileHelper::getFullPath("resources\\font\\sysongti.ttf").c_str(), 16.0f, NULL,
		//	io.Fonts->GetGlyphRangesChineseFull());

		//Config::FontDefault = io.Fonts->AddFontFromFileTTF(FileHelper::getFullPath("resources\\font\\sysongti.ttf").c_str(), 16.0f, NULL,
		//	io.Fonts->GetGlyphRangesChineseFull());
		Config::FontMiddle = Config::FontSmall = Config::FontDefault = Config::FontBig;
		io.FontDefault = Config::FontDefault;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		setDarkThemeColors();

		Application& app = Application::instance();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

		Config::ScreenWidth = GetSystemMetrics(SM_CXSCREEN);//获取显示器的宽
		Config::ScreenHeigth = GetSystemMetrics(SM_CYSCREEN);//获取显示器的高
		glfwGetWindowPos(window, &Config::WindowPosX, &Config::WindowPosY);  // 获取窗口的位置
		glfwGetWindowSize(window, &Config::WindowWidth, &Config::WindowHeigth);  // 获取窗口的宽高

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 330");
	}

	void ImGuiLayer::shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImPlot::DestroyContext();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::onEvent(Event& e)
	{
		if (_blockEvents)
		{
			ImGuiIO& io = ImGui::GetIO();
			e.handled |= e.isInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.handled |= e.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::begin()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		// 设置视图窗口可停靠
		ImGui::DockSpaceOverViewport();
	}

	void ImGuiLayer::end()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::instance();
		io.DisplaySize = ImVec2((float)app.getWindow().getWidth(), (float)app.getWindow().getHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::setDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = Config::ImGuiColWindowBg;

		// Headers
		colors[ImGuiCol_Header] = Config::ImGuiColHeader;
		colors[ImGuiCol_HeaderHovered] = Config::ImGuiColHeaderHovered;
		colors[ImGuiCol_HeaderActive] = Config::ImGuiColHeaderActive;

		// Buttons
		colors[ImGuiCol_Button] = Config::ImGuiColButton;
		colors[ImGuiCol_ButtonHovered] = Config::ImGuiColButtonHovered;
		colors[ImGuiCol_ButtonActive] = Config::ImGuiColButtonActive;

		// Frame BG
		colors[ImGuiCol_FrameBg] = Config::ImGuiColFrameBg;
		colors[ImGuiCol_FrameBgHovered] = Config::ImGuiColFrameBgHovered;
		colors[ImGuiCol_FrameBgActive] = Config::ImGuiColFrameBgActive;

		// Tabs
		colors[ImGuiCol_Tab] = Config::ImGuiColTab;
		colors[ImGuiCol_TabHovered] = Config::ImGuiColTabHovered;
		colors[ImGuiCol_TabActive] = Config::ImGuiColTabActive;
		colors[ImGuiCol_TabUnfocused] = Config::ImGuiColTabUnfocused;
		colors[ImGuiCol_TabUnfocusedActive] = Config::ImGuiColTabUnfocusedActive;

		// Title
		colors[ImGuiCol_TitleBg] = Config::ImGuiColTitleBg;
		colors[ImGuiCol_TitleBgActive] = Config::ImGuiColTitleBgActive;
		colors[ImGuiCol_TitleBgCollapsed] = Config::ImGuiColTitleBgCollapsed;
	}

}
