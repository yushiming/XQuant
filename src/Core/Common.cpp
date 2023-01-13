#include "Core/Common.h"

namespace XQuant {



	std::map<EPlatform, std::string> PlatformName = {
		{EPlatform::eNull, u8"无"},
		{EPlatform::eFutures, u8"期货(CTP)"},
		{EPlatform::eStocks, u8"股票(XTP)"},
		{EPlatform::eForex, u8"外汇(IF)"},
		{EPlatform::eDigitalCash, u8"数字货币(Binance)"}
	};

	std::map<ETrendTime, std::string> TrendTimeName = {
		{ETrendTime::eTTNull, u8"无"},
		{ETrendTime::eTTTimeShare, u8"分时"},
		{ETrendTime::eTTDay, u8"日"},
		{ETrendTime::eTTWeek, u8"周"},
		{ETrendTime::eTTMonth, u8"月"},
		{ETrendTime::eTTSeason, u8"季"},
		{ETrendTime::eTTYear, u8"年"},
		{ETrendTime::eTT1Minute, u8"1分"},
		{ETrendTime::eTT5Minute, u8"5分"},
		{ETrendTime::eTT15Minute, u8"15分"},
		{ETrendTime::eTT30Minute, u8"30分"},
		{ETrendTime::eTT60Minute, u8"60分"}
	};

	std::vector<std::string> sectionTypesEN = {
		"gratis",
		"tactics",
		"climbing",
		"blockPoint"
	};
}