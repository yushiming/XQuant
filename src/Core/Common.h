
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
#include <map>

namespace XQuant {

	enum EPlatform : int {
		eNull             = 0,            // 
		eFutures          = 1,            // 期货
		eStocks           = 2,            // 股票
		eForex            = 3,            // 外汇
		eDigitalCash      = 4,            // 数字货币
	};

	extern std::map<EPlatform, std::string> PlatformName;


	enum EChapterType : int {
		eChapterTypeNull = -1,
		eMainLine = 0,		//	主线
		eClimbingTower = 1,		//	爬塔
		eChapterTypeCount = 2,
	};

	extern std::vector<std::string> sectionTypesEN;

}