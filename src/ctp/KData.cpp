// KData.cpp : Defines the entry point for the DLL application.
//
#define _CRT_SECURE_NO_WARNINGS
#include "KData.h"
#include"Strategy.h"

extern Strategy* g_strategy;//策略类指针
//产品交易时段映射
std::map<std::string, std::map<int, ProductTradeTime> > TodayProductTradeTime;

//0900,四位时分格式转换为秒
int TimeIntervalToSeconds(const char* time) {
	char buf[5];
	int h, mi, s;
	buf[0] = time[0];
	buf[1] = time[1];
	buf[2] = '\0';
	h = atoi(buf);

	buf[0] = time[2];
	buf[1] = time[3];
	buf[2] = '\0';
	mi = atoi(buf);
	if (h <= 18)
		return (h + 24) * 3600 + mi * 60  ;
	else
		return h * 3600 + mi * 60  ;
}
//09:00:00,六位时分秒格式转换为秒
int ToSeconds(const char * time){
	char buf[5];
	int h,mi,s;
	buf[0]=time[0];
	buf[1]=time[1];
	buf[2]='\0';
	h = atoi(buf);

	buf[0]=time[3];
	buf[1]=time[4];
	buf[2]='\0';
	mi = atoi(buf);

	buf[0]=time[6];
	buf[1]=time[7];
	buf[2]='\0';
	s = atoi(buf);

	if(h<=18)
	return (h+24)*3600+mi*60+s;
	else
		return h* 3600 + mi * 60 + s;
}

//09:00:00,六位时分秒格式转换为090000
int TimeToInt(const char* time) {
	char buf[5];
	int h, mi, s;
	buf[0] = time[0];
	buf[1] = time[1];
	buf[2] = '\0';
	h = atoi(buf);

	buf[0] = time[3];
	buf[1] = time[4];
	buf[2] = '\0';
	mi = atoi(buf);

	buf[0] = time[6];
	buf[1] = time[7];
	buf[2] = '\0';
	s = atoi(buf);
		return h * 10000 + mi * 100 + s;
}

long GetNextDate(long date)
{
	const uint32_t ONE_DAY_SECS = 24 * 60 * 60;
	int day = date % 100;
	int year = date / 10000, month = (date - day) % 1000/100;
	if (year == 0)
		return -1;
		struct tm t;
	t.tm_year = year - 1900;
	t.tm_mon = month - 1;
	t.tm_mday = day;
	t.tm_hour = 1; t.tm_min = 0; t.tm_sec = 0;
	uint64_t next_day_timestamp = mktime(&t) + ONE_DAY_SECS;
	struct tm* time_info = localtime((time_t*)&next_day_timestamp);
	return ((time_info->tm_year + 1900) * 10000 + (time_info->tm_mon + 1) * 100 + time_info->tm_mday);
}

//获取当地时间的秒数
int GetLocalTimeSeconds()
{
	time_t tt = time(NULL);
	long timezone;
	_get_timezone(&timezone);
	return (tt - timezone);
}
//获取当地时间的时分秒，如09:00:00转为090000
int GetLocalTime()
{
	time_t tt = time(NULL);
	long timezone;
	/*_get_timezone(&timezone);

	tt = (tt + timezone);*/
	tm* t = localtime(&tt);
	
	return  t->tm_hour * 10000 + t->tm_min * 100 + t->tm_sec;
}
int GetLocalDate()
{
	time_t tt = time(NULL);
	tm* t = localtime(&tt);
	printf("%d-%02d-%02d %02d:%02d:%02d\n",
		t->tm_year + 1900,
		t->tm_mon + 1,
		t->tm_mday,
		t->tm_hour,
		t->tm_min,
		t->tm_sec);

	return ((t->tm_year + 1900) * 1000 + (t->tm_mon + 1) * 100 + t->tm_mday);
}
char * ToTimeString(int seconds,char * pszTime){
	int h = seconds/3600%24;
	int minute = seconds%3600/60;
	int second = seconds%3600%60;

	sprintf(pszTime,"%02d:%02d:%02d",h,minute,second);
	return pszTime;
}

const char* GetExchange(const char* pszInstrumentID)
{
	char ch1 = pszInstrumentID[0];
	char ch2 = pszInstrumentID[1];

	switch (ch1) {
	case 'a':
		switch (ch2) {
		case 'g': //ag
		case 'l': //al
		case 'u': //au
			return "SHFE";
		default:
			return "DCE";
		};
		break;
	case 'b':
		switch (ch2) {
		case 'c': //bc
			return "INE";
		case 'u': //bu
			return "SHFE";
		default://b  bb
			return "DCE";
		};
		break;
	case 'c':
		switch (ch2) {
		case 'u':	//cu
			return "SHFE";
		default:// c cs
			return "DCE";
		};
		break;
	case 'e'://eb eg
		return "DCE";
	case 'f'://fb fu
		switch (ch2) {
		case 'b':	//fb
			return "DCE";
		case 'u':	//fu
			return "SHFE";
		};
		break;
	case 'h'://hc
		return "SHFE";
	case 'i'://i
		return "DCE";
	case 'j'://j jd jm
		return "DCE";
	case 'l':
		switch (ch2) {
		case 'u':	//lu
			return "INE";
		case 'h'://lh
		default://l
			return "DCE";
		};
	case 'm':
		return "DCE";
	case 'n':
		switch (ch2) {
		case 'i':	//ni
			return "SHFE";
		case 'r':	//lu
			return "INE";
		};
		break;
	case 'p':
		switch (ch2) {
		case 'b':	//pb
			return "SHFE";
		default://p pp pg
			return "DCE";
		};
		break;
	case 'r':
		switch (ch2) {
		case 'b':	//rb
		case 'u':
			return "SHFE";
		case 'r'://rr
			return "DCE";
		};
		break;
	case 's':
		switch (ch2) {
		case 'c':	//sc
			return "INE";
		case 'n'://sn
		case 'p'://sp
		case 's'://ss
			return "SHFE";
		};
		break;
	case 'v':
		return "DCE";
	case 'w':
		switch (ch2) {
		case 'r':	//wr
			return "SHFE";
		};
		break;
	case 'y':
		return "DCE";
	case 'z':
		switch (ch2) {
		case 'n':	//zn
			return "SHFE";
		};
		break;
	case 'I'://IF;
		switch (ch2) {
		case 'C':
		case 'F':
		case 'H':
			return "CFFEX";
		};
		break;
	case 'T':
		switch (ch2) {
		case 'A'://TA
			return "CZCE";
		case 'F'://TF
		case 'S'://TS
		default://T
			return "CFFEX";
		};
	case 'A'://AP
	case 'C'://CF CJ CY
	case 'F'://FG
	case 'J'://JR
	case 'L'://LR
	case 'M'://MA
	case 'O'://OI
	case 'P'://PF PM
	case 'R'://RI RM RS
	case 'S'://SA SF SM SR
	case 'U'://UR
	case 'W'://WH
	case 'Z'://ZC
		return "CZCE";
	}
	return "";
}

CKDataCollection::CKDataCollection(void){

}
void CKDataCollection::AddTickData(CThostFtdcDepthMarketDataField* pTickData) {
	map<string, CThostFtdcDepthMarketDataField*>::iterator itd = m_MarketData.find(pTickData->InstrumentID);
	if (itd == m_MarketData.end()) {
		CThostFtdcDepthMarketDataField* pNew = new CThostFtdcDepthMarketDataField();
		memcpy(pNew, pTickData, sizeof(CThostFtdcDepthMarketDataField));
		m_MarketData.insert(pair<string, CThostFtdcDepthMarketDataField*>(pTickData->InstrumentID, pNew));
	}
	else {
		//delete itd->second;
		memcpy(itd->second, pTickData, sizeof(CThostFtdcDepthMarketDataField));
	}

	map<string, CKBarSeries*>* pMap;
	map<string, map<string, CKBarSeries*>*>::iterator it = m_Data.find(pTickData->InstrumentID);
	if (it != m_Data.end()) {
		pMap = it->second;

		map<string, CKBarSeries*>::iterator it2 = pMap->begin();
		map<string, CKBarSeries*>::iterator end2 = pMap->end();
		while (it2 != end2) {
			CKBar* pBar = it2->second->AddTickData(pTickData);
			//生成了k线
			if (pBar != NULL) {
				//bar closed;
				//todo process bar;

				printf("%s close at %f on %s\n", pBar->m_sInstrumentID, pBar->m_fClose, pBar->m_sEndTime);
				std::string  str = it2->second->m_sDisplayName;
				g_strategy->OnBar(pBar, it2->second);
				pBar->SaveKBar("E:/database/Kbar/" + str);
				pBar->ShowBar();
			}
			it2++;
		}
	}
}

int CKDataCollection::ReadKBarSeries(std::string strPath, CKBarSeries* pBarSeries)
{

	char filePath[100] = { '\0' };
	//strcpy(filePath, strPath.c_str());
	//生成文件的路径
	sprintf_s(filePath, sizeof(filePath), "%s.csv", strPath.c_str());//用string作为参数，有时候会有问题，尽量用const char *
	//定义一个文件的输入流
	std::ifstream inFile;
	inFile.open(filePath, ios::in);
	if (!inFile)
	{
		std::cerr << "打开文件失败" << std::endl;
		return -1;
	}
	char dataLine[1024];

	int n = 0;
	pBarSeries->GetCount(&n);
	int i = 0;
	int nMax = pBarSeries->m_nTotal;

	int nMulti = pBarSeries->m_nMultiply;
	int nInterval = pBarSeries->m_nInterval;
	BarType bt = pBarSeries->m_BarType;

	int nReadCount = 0;
	while (inFile.getline(dataLine, sizeof(dataLine), '\n') && n < nMax)
	{
		if (i > 0)
		{
			CKBar* pbar = new CKBar;
			pbar->m_BarType = bt;
			pbar->m_nInterval = nInterval;
			pbar->m_nMultiply = nMulti;
			pbar->m_nIndex = n;
			pBarSeries->ReadKbar(dataLine, *pbar);
			pBarSeries->AddKBar(pbar);
			g_strategy->OnBar(pbar, pBarSeries);
			nReadCount++;
			n++;
		}
		i++;
	}
	inFile.close();
	if (nReadCount)
		return 0;
	return -1;
}

const char * CKBar::GetSeriesName()
{
	return ((CKBarSeries *)m_pSeries)->m_sDisplayName;
}

CThostFtdcDepthMarketDataField * CKDataCollection::GetMarketData(const char *pszCode)
{
	map<string,CThostFtdcDepthMarketDataField *>::iterator it = m_MarketData.find(pszCode);
	if(it == m_MarketData.end()){
		return NULL;
	}else{
		return it->second;
	}
}

void ReadProductID(std::string& instStr, std::vector<std::string>& instVec)
{
	char tmp[256] = { '\0' };
	strcpy(tmp, instStr.c_str());
	char* p = nullptr;
	p = strtok(tmp, " ");
	while (p)
	{
		std::string  str = p;
		instVec.push_back(str);
		p = strtok(NULL, " ");
	}
}

void ReadTimeGap(std::string& timeStr, std::map<int, ProductTradeTime>& timeMap)
{
	char tmp[256] = { '\0' };
	strcpy(tmp, timeStr.c_str());
	char* p = nullptr;
	p = strtok(tmp, " ");
	int i = 0;
	while (p)
	{
		std::string str, str1, str2;
		str = p;
		if (str.length() > 8)
		{
			str1 = str.substr(0, 4);
			str2 = str.substr(5, 4);
			ProductTradeTime ptt;
			ptt.BeginTime = TimeIntervalToSeconds(str1.c_str());
			switch (ptt.BeginTime)
			{
			case 21 * 3600:
				i = 1;
				break;
			case 33 * 3600:
			case 33 * 3600 + 900:
			case 33 * 3600 + 1800:
				i = 2;
				break;
			case 34 * 3600 + 1800:
				i = 3;
				break;
			case 37 * 3600:
			case 37 * 3600 + 1800:
				i = 4;
			default:
				break;
			}
			ptt.TradeTimeInterval = (ProductTradingTimeType)i;
			ptt.EndTime = TimeIntervalToSeconds(str2.c_str());
			timeMap.insert(make_pair(i, ptt));
		}
		p = strtok(NULL, " ");
	}
}
void ClearTradeTimeMap()
{
	//产品交易时段映射
	//std::map<std::string, std::map<int, ProductTradeTime> > TodayProductTradeTime;
	auto it = TodayProductTradeTime.begin();
	auto end = TodayProductTradeTime.end();
	while (it != end)
	{
		it->second.clear();
		it++;
	}
	TodayProductTradeTime.clear();
}
//map<std::string, std::string>& tradetimemap
void ReadTradeTimeMap()
{
	std::ifstream file1(".\\config\\TradingTime.txt", ios::in);
	string fieldKey;
	string fieldValve;
	/*std::vector<std::string> instVec;
	std::map<int, ProductTradeTime> instTradeTime;*/
	char dataLine[256];
	if (!file1)
	{
		std::cout << "配置文件不存在" << endl;
		return;
	}
	else
	{
		while (file1.getline(dataLine, sizeof(dataLine), '\n'))
		{
			std::vector<std::string> instVec;
			std::map<int, ProductTradeTime> instTradeTime;
			
			int length = strlen(dataLine);
			if (length == 0 || (length > 0 && dataLine[0] == '#'))
				continue;
			char tmp[128];
			for (int i = 0, j = 0, count = 0; i < length + 1; i++)
			{
				if (dataLine[i] != ',' && dataLine[i] != '\0')
					tmp[j++] = dataLine[i];
				else
				{
					tmp[j] = '\0';

					count++;
					//cout << "count: " << count << ",tmp :" << tmp << endl;
					j = 0;
					switch (count)
					{
					case 1:
						fieldKey = tmp;
						break;
					case 2:
						fieldValve = tmp;
					default:
						break;
					}
				}
			}//for结束
			ReadProductID(fieldKey, instVec);

			ReadTimeGap(fieldValve, instTradeTime);
			if (instVec.size() > 0)
			{
				for (auto it = instVec.begin(); it != instVec.end(); it++)
					TodayProductTradeTime.insert(make_pair(*it, instTradeTime));
			}

		}
	}


	file1.close();
}

void CKBarSeries::ReadKbar(const char* barStr, CKBar& bar)
{
	char temp[1024] = { '\0' };
	strcpy(temp, barStr);
	char* p = nullptr;
	//strtok,找到了分隔符就会用'\0'替换
	p=strtok(temp, ",");
	int i = 0;
	while (p)
	{
		switch (i)
		{
		case 0:
			strcpy(bar.m_sInstrumentID, p);
			break;
		case 1:
			strcpy(bar.m_sActionDay, p);
			break;
		case 2:
			strcpy(bar.m_sTradingDay, p);
			break;
		case 3:
			strcpy(bar.m_sBeginTime, p);
			break;
		case 4:
			strcpy(bar.m_sEndTime, p);
			break;
		case 5:
			bar.m_fOpen = atof(p);
			break;
		case 6:
			bar.m_fHighest = atof(p);
			break;
		case 7:
			bar.m_fLowest = atof(p);
			break;
		case 8:
			bar.m_fClose = atof(p);
			break;
		case 9:
			bar.m_fAverage = atof(p);
			break;
		case 10:
			bar.m_nVolume = atoi(p);
			break;
		case 11:
			bar.m_nPosition = atoi(p);
			break;
		case 12:
			bar.m_nPositionDiff = atoi(p);
			bar.m_nStatus = 1;
			break;
		default:
			break;
		}
		p = strtok(NULL, ",");
		i++;
	}

}
