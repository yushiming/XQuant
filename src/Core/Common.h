
#pragma once

//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  Common.h
//
//   Description:  公共数据类、包含各类数据
//
//       Version:  1.0
//       Created:  2023年1月2日 12时52分34秒
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
		eMainLine = 0,		//	主线
		eClimbingTower = 1,		//	爬塔
		eChapterTypeCount = 2,
	};

	extern std::vector<std::string> sectionTypesEN;

}