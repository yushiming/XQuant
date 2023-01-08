#include "Language/MultiLanguage.h"

namespace XQuant {

	LangType MultiLanguage::type = LangType::eChinese;

	const std::map<int, std::string>* MultiLanguage::curLanguage;

	const std::map<int, std::string>* MultiLanguage::chinese = new std::map<int, std::string>{
		{eStrQuantAutoTraderSoft       ,"�����Զ��������"},
		{eStrCurData					,"��ǰ����"}
	};


	const std::map<int, std::string>* MultiLanguage::english = new std::map<int, std::string >{
		{eStrQuantAutoTraderSoft,		 "QuantAutoTraderSoft"}

	};

}
