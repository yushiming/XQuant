#include "Core/Common.h"

namespace XQuant {

	std::map<EPlatform, std::string> PlatformName = {
		{EPlatform::eNull, u8"无"},
		{EPlatform::eFutures, u8"期货(CTP)"},
		{EPlatform::eStocks, u8"股票(XTP)"},
		{EPlatform::eForex, u8"外汇(IF)"},
		{EPlatform::eDigitalCash, u8"数字货币(Binance)"}
	};

	std::vector<std::string> sectionTypesEN = {
		"gratis",
		"tactics",
		"climbing",
		"blockPoint"
	};
}