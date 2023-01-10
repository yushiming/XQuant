
#pragma once

//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  Config.h
//
//   Description:  配置数据
//
//       Version:  1.0
//       Created:  2023年1月7日 15时19分56秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================


#include <vector>
#include <string>

#include <imgui.h>

namespace XQuant {

	class Config {
	public:

		static int WindowPosX;         // 窗口x坐标
		static int WindowPosY;         // 窗口y坐标

		static int WindowWidth;        // 窗口宽度
		static int WindowHeigth;       // 窗口高度

		static int ScreenWidth;        // 屏幕宽度
		static int ScreenHeigth;       // 屏幕高度

		static ImFont* FontBig;
		static ImFont* FontMiddle;
		static ImFont* FontSmall;
		static ImFont* FontDefault;

		static ImVec4 ImGuiColWindowBg;

		// Headers
		static ImVec4 ImGuiColHeader;
		static ImVec4 ImGuiColHeaderHovered;
		static ImVec4 ImGuiColHeaderActive;

		// Buttons
		static ImVec4 ImGuiColButton;
		static ImVec4 ImGuiColButtonHovered;
		static ImVec4 ImGuiColButtonActive;

		// Frame BG
		static ImVec4 ImGuiColFrameBg;
		static ImVec4 ImGuiColFrameBgHovered;
		static ImVec4 ImGuiColFrameBgActive;

		// Tabs
		static ImVec4 ImGuiColTab;
		static ImVec4 ImGuiColTabHovered;
		static ImVec4 ImGuiColTabActive;
		static ImVec4 ImGuiColTabUnfocused;
		static ImVec4 ImGuiColTabUnfocusedActive;

		// Title
		static ImVec4 ImGuiColTitleBg;
		static ImVec4 ImGuiColTitleBgActive;
		static ImVec4 ImGuiColTitleBgCollapsed;

	};

}