#include "Core/Config.h"

namespace XQuant {

	int Config::WindowPosX = 0;
	int Config::WindowPosY = 0;

	int Config::WindowWidth = 0;
	int Config::WindowHeigth = 0;

	int Config::ScreenWidth = 0;
	int Config::ScreenHeigth = 0;

	ImFont* Config::FontBig = nullptr;
	ImFont* Config::FontMiddle = nullptr;
	ImFont* Config::FontSmall = nullptr;
	ImFont* Config::FontDefault = nullptr;

	ImVec4 Config::ImGuiColWindowBg = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	ImVec4 Config::ImGuiColHeader = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	ImVec4 Config::ImGuiColHeaderHovered = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	ImVec4 Config::ImGuiColHeaderActive = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	ImVec4 Config::ImGuiColButton = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	ImVec4 Config::ImGuiColButtonHovered = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	ImVec4 Config::ImGuiColButtonActive = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	ImVec4 Config::ImGuiColFrameBg = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	ImVec4 Config::ImGuiColFrameBgHovered = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	ImVec4 Config::ImGuiColFrameBgActive = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	ImVec4 Config::ImGuiColTab = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	ImVec4 Config::ImGuiColTabHovered = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	ImVec4 Config::ImGuiColTabActive = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	ImVec4 Config::ImGuiColTabUnfocused = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	ImVec4 Config::ImGuiColTabUnfocusedActive = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	ImVec4 Config::ImGuiColTitleBg = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	ImVec4 Config::ImGuiColTitleBgActive = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	ImVec4 Config::ImGuiColTitleBgCollapsed = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}