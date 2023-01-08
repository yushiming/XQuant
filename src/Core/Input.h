//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  Input.h
//
//   Description:  输入封装、包含按键及鼠标操作
//
//       Version:  1.0
//       Created:  2023年1月2日 14时43分52秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================


#pragma once

#include <glm/glm.hpp>

#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

namespace XQuant {

	class Input
	{
	public:
		static bool isKeyPressed(KeyCode key);

		static bool isMouseButtonPressed(MouseCode button);
		static glm::vec2 getMousePosition();
		static float getMouseX();
		static float getMouseY();
	};
}
