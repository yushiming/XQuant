#include "Core/Common.h"

namespace XQuant {



	std::map<EPlatform, std::string> PlatformName = {
		{EPlatform::eNull, u8"��"},
		{EPlatform::eFutures, u8"�ڻ�(CTP)"},
		{EPlatform::eStocks, u8"��Ʊ(XTP)"},
		{EPlatform::eForex, u8"���(IF)"},
		{EPlatform::eDigitalCash, u8"���ֻ���(Binance)"}
	};

	std::map<ETrendTime, std::string> TrendTimeName = {
		{ETrendTime::eTTNull, u8"��"},
		{ETrendTime::eTTTimeShare, u8"��ʱ"},
		{ETrendTime::eTTDay, u8"��"},
		{ETrendTime::eTTWeek, u8"��"},
		{ETrendTime::eTTMonth, u8"��"},
		{ETrendTime::eTTSeason, u8"��"},
		{ETrendTime::eTTYear, u8"��"},
		{ETrendTime::eTT1Minute, u8"1��"},
		{ETrendTime::eTT5Minute, u8"5��"},
		{ETrendTime::eTT15Minute, u8"15��"},
		{ETrendTime::eTT30Minute, u8"30��"},
		{ETrendTime::eTT60Minute, u8"60��"}
	};

	std::vector<std::string> sectionTypesEN = {
		"gratis",
		"tactics",
		"climbing",
		"blockPoint"
	};
}