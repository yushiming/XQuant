//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  MultiLanguage.h
//
//   Description:  �����Դ���
//
//       Version:  1.0
//       Created:  2023��1��3�� 17ʱ35��56��
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================

#pragma once

#include <map>
#include <string>

namespace XQuant {

	enum LangType {
		eChinese,
		eEnglish
	};

	enum StrFlag {
		eStrQuantAutoTraderSoft,							//�ؿ��༭��
		eStrCurData
	
	};


	class MultiLanguage {

	public:
		static void choiceLanguage(LangType type) {
			if (type == LangType::eChinese) {
				curLanguage = chinese;
			}
			else if (type == LangType::eEnglish) {
				curLanguage = english;
			}
		}

		static std::string utf8(int idx) {
			return curLanguage->at(idx);
		}

	private:
		static LangType type;
		static const std::map<int, std::string>* curLanguage;
		static const std::map<int, std::string>* chinese;
		static const std::map<int, std::string>* english;

	};

}
