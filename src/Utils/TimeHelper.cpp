#include <sstream>
#include <ctime> 
#include <time.h>
#include <process.h>

#include "TimeHelper.h"

namespace XQuant {

	std::string TimeHelper::getCurrentTimeStr() {
		// ���ڵ�ǰϵͳ�ĵ�ǰ����/ʱ��
		time_t time_seconds = time(0); //��ȡ��1970������˶����룬����time_t���͵�timep
		struct tm ptm;
		localtime_s(&ptm, &time_seconds);//��localtime������ת��Ϊstruct tm�ṹ��

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
