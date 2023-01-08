#include <sstream>
#include <functional>

#include "StringHelper.h"

namespace XQuant {


	std::vector<std::string> StringHelper::split(const std::string& str,
			const std::string& sep,
			int limit)
	{
		std::vector<std::string> ret;
		split(str, sep, limit, [&](const std::string& s, int i) {
			ret.push_back(s);
			});
		return ret;
	}

	size_t StringHelper::split(const std::string& str,
		const std::string& sep,
		int limit,
		const std::function<void(const std::string&, int)>& callback)
	{
		size_t pos1 = 0, pos2 = 0;
		int n = 0;
		while (true) {
			if (limit > 0 && n + 1 >= limit) {
				pos2 = std::string::npos;
				break;
			}
			pos2 = str.find_first_of(sep, pos1);
			if (pos2 == std::string::npos) {
				break;
			}
			if (callback != nullptr) {
				callback(str.substr(pos1, pos2 - pos1), n);
			}
			n++;
			pos1 = pos2 + 1;
		}
		if (pos1 != std::string::npos) {
			if (callback != nullptr) {
				callback(str.substr(pos1), n);
			}
			n++;
		}
		return n;
	}

	std::string StringHelper::ltrim(const std::string& str)
	{
		size_t p = str.find_first_not_of(" \t\f\v\n\r");
		if (p == std::string::npos) {
			return "";
		}
		return str.substr(p);
	}

	std::string StringHelper::rtrim(const std::string& str)
	{
		size_t p = str.find_last_not_of(" \t\f\v\n\r");
		if (p == std::string::npos) {
			return "";
		}
		return str.substr(0, p + 1);
	}

	std::string StringHelper::trim(const std::string& str)
	{
		size_t p1 = str.find_first_not_of(" \t\f\v\n\r");
		size_t p2 = str.find_last_not_of(" \t\f\v\n\r");
		if (p1 == std::string::npos || p2 == std::string::npos) {
			return "";
		}
		return str.substr(p1, p2 + 1 - p1);
	}

	//std::string StringHelper::getStringMD5Hash(const std::string& data)
	//{
	//	static const unsigned int MD5_DIGEST_LENGTH = 16;

	//	if (data.length() == 0)
	//	{
	//		return std::string();
	//	}

	//	md5_state_t state;
	//	md5_byte_t digest[MD5_DIGEST_LENGTH];
	//	char hexOutput[(MD5_DIGEST_LENGTH << 1) + 1] = { 0 };

	//	md5_init(&state);
	//	md5_append(&state, (const md5_byte_t*)data.c_str(), (int)data.length());
	//	md5_finish(&state, digest);

	//	for (int di = 0; di < 16; ++di)
	//		sprintf(hexOutput + di * 2, "%02x", digest[di]);

	//	return hexOutput;
	//}

	std::string StringHelper::concat(const std::vector<std::string>& strArr, const std::string& sep){
		std::stringstream ss;
		for (size_t i = 0; i < strArr.size(); i++) {
			ss << strArr[i];
			if (i < strArr.size() - 1) {
				ss << sep;
			}
		}

		return ss.str();
	}



}