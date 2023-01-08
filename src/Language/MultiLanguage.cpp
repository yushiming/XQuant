#include "Language/MultiLanguage.h"

namespace XQuant {

	LangType MultiLanguage::type = LangType::eChinese;

	const std::map<int, std::string>* MultiLanguage::curLanguage;

	const std::map<int, std::string>* MultiLanguage::chinese = new std::map<int, std::string>{
		{eStrQuantAutoTraderSoft       ,"量化自动交易软件"},
		{eStrCurData					,"当前数据"}
	};


	const std::map<int, std::string>* MultiLanguage::english = new std::map<int, std::string >{
		{eStrQuantAutoTraderSoft,		 "QuantAutoTraderSoft"}

	};

}
