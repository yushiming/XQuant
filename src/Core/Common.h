
#pragma once

//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  Common.h
//
//   Description:  ���������ࡢ������������
//
//       Version:  1.0
//       Created:  2023��1��2�� 12ʱ52��34��
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================


#include <vector>
#include <string>

namespace XQuant {

	enum EChapterType : int {
		eChapterTypeNull = -1,
		eMainLine = 0,		//	����
		eClimbingTower = 1,		//	����
		eChapterTypeCount = 2,
	};

	extern std::vector<std::string> sectionTypesEN;

}