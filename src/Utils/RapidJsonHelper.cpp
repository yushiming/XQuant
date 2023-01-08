#include "RapidJsonHelper.h"

#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace rapidjson;

namespace XQuant {

	const rapidjson::Value* RapidJsonHelper::jsonMember(const rapidjson::Value* dict, const char* key) {
		if (!dict) {
			return nullptr;
		}
		auto iter = dict->FindMember(key);
		if (iter == dict->MemberEnd()) {
			return nullptr;
		}
		return &(iter->value);
	}


	const rapidjson::Value* RapidJsonHelper::jsonHasMember(const rapidjson::Value* dict, const char* key) {
		if (dict && dict->HasMember(key)) {
			return &((*dict)[key]);
		}
		return nullptr;
	}

	const char* RapidJsonHelper::jsonToString(const rapidjson::Value* value, const char* defaultValue/* = NULL*/) {
		if (value && value->IsString()) {
			return value->GetString();
		}
		return defaultValue;
	}

	int RapidJsonHelper::jsonToInt(const rapidjson::Value* value, int defaultValue/* = 0*/) {
		if (value && value->IsInt()) {
			return value->GetInt();
		}
		return defaultValue;
	}

	double RapidJsonHelper::jsonToNumber(const rapidjson::Value* value, double defaultValue/* = 0.0*/) {
		if (value && value->IsNumber()) {
			return value->GetDouble();
		}
		return defaultValue;
	}

	bool RapidJsonHelper::jsonToBool(const rapidjson::Value* value, bool defaultValue/* = false*/) {
		if (value && value->IsBool()) {
			return value->GetBool();
		}
		return defaultValue;
	}

	const rapidjson::Value* RapidJsonHelper::jsonToArray(const rapidjson::Value* value) {
		if (value && value->IsArray()) {
			return value;
		}
		return nullptr;
	}

	const rapidjson::Value* RapidJsonHelper::jsonToObject(const rapidjson::Value* value) {
		if (value && value->IsObject()) {
			return value;
		}
		return nullptr;
	}

	std::string RapidJsonHelper::jsonValueToString(const rapidjson::Value* valObj) {

		rapidjson::StringBuffer strBuf;
		rapidjson::Writer<rapidjson::StringBuffer> writer(strBuf);
		valObj->Accept(writer);
		return std::string(strBuf.GetString());
	}

}