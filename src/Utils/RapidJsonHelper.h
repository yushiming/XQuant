//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  RapidJsonHelper.h
//
//   Description:  RapidJson库的封装
//
//       Version:  1.0
//       Created:  2023年1月3日 16时26分46秒
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================
#include <string>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"

namespace XQuant {

	class RapidJsonHelper {
	public:
		static const rapidjson::Value* jsonMember(const rapidjson::Value* dict, const char* key);

		// @brief 和上面函数作用相同 只是查找方式不同
		static const rapidjson::Value* jsonHasMember(const rapidjson::Value* dict, const char* key);

		static const char* jsonToString(const rapidjson::Value* value, const char* defaultValue = nullptr);

		static int jsonToInt(const rapidjson::Value* value, int defaultValue = 0);

		static double jsonToNumber(const rapidjson::Value* value, double defaultValue = 0.0);

		static bool jsonToBool(const rapidjson::Value* value, bool defaultValue = false);

		static const rapidjson::Value* jsonToArray(const rapidjson::Value* value);

		static const rapidjson::Value* jsonToObject(const rapidjson::Value* value);

		static std::string jsonValueToString(const rapidjson::Value* valObj);
	};


}