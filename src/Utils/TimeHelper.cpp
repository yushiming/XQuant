#include <sstream>
#include <ctime> 
#include <time.h>
#include <process.h>

#include "TimeHelper.h"

namespace XQuant {

	std::string TimeHelper::getCurrentTimeStr() {
		// 基于当前系统的当前日期/时间
		time_t time_seconds = time(0); //获取从1970至今过了多少秒，存入time_t类型的timep
		struct tm ptm;
		localtime_s(&ptm, &time_seconds);//用localtime将秒数转化为struct tm结构体

		std::stringstream time;
		time << ptm.tm_year + 1900 << "/";
		time << ptm.tm_mon + 1 << "/";
		time << ptm.tm_mday << "/ ";
		time << ptm.tm_hour << ":";
		time << ptm.tm_min << ":";
		time << ptm.tm_sec;

		return time.str();
	}


}
