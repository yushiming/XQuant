//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  MouseCodes.h
//
//   Description:  ������
//
//       Version:  1.0
//       Created:  2023��1��2�� 15ʱ56��46��
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================


#pragma once

namespace XQuant {
	using MouseCode = uint16_t;
	namespace Mouse
	{
		enum MouseCode : uint16_t {
			eButton0 = 0,
			eButton1 = 1,
			eButton2 = 2,
			eButton3 = 3,
			eButton4 = 4,
			eButton5 = 5,
			eButton6 = 6,
			eButton7 = 7,

			eButtonLast = eButton7,
			eButtonLeft = eButton0,
			eButtonRight = eButton1,
			eButtonMiddle = eButton2
		};
	}
}











