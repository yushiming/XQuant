//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  Input.h
//
//   Description:  �����װ������������������
//
//       Version:  1.0
//       Created:  2023��1��2�� 14ʱ43��52��
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
