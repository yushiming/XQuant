#include "Core/Common.h"

namespace XQuant {

	std::map<EPlatform, std::string> PlatformName = {
		{EPlatform::eNull, u8"��"},
		{EPlatform::eFutures, u8"�ڻ�(CTP)"},
		{EPlatform::eStocks, u8"��Ʊ(XTP)"},
		{EPlatform::eForex, u8"���(IF)"},
		{EPlatform::eDigitalCash, u8"���ֻ���(Binance)"}
	};

	std::vector<std::string> sectionTypesEN = {
		"gratis",
		"tactics",
		"climbing",
		"blockPoint"
	};
}