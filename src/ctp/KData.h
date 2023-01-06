
// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the KDATA_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// KDATA_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.

#ifndef KDATA_H_
#define KDATA_H_

#ifdef WIN32
  #pragma warning (disable: 4514 4786 4503 C4996)
#endif

#include "./ThostTraderApi/ThostFtdcUserApiStruct.h"
//#include "InstrumentMarketData.h"

#include <map>
#include <string>

#include<fstream>
#include<vector>
#include<iostream>

#include<time.h>

using namespace std;

extern char g_cPreTradingDay[9];
extern char g_cNextTradeDate[9];

//extern Strategy* g_strategy;//策略类指针
//日期类型
enum TradingDayType {
	NormalWorkingDay1,
	NormalWorkingDay2_5,
	//BeforeHolidayWorkingDay,
	AfterHolidayWorkingDay,
	CFFEXLastTradingDay
};

//产品交易时段枚举
enum ProductTradingTimeType {
	NoTrading = 0										//非交易时段
	, NightPartOne										//夜盘
	, AMPartOne											//上午第一阶段
	, AMPartTwo											//上午第二阶段
	, PMPartOne											//下午第一阶段
	, TradeTimeIntervalCnt                                // 有多少个交易时段
};


//产品交易时段结构体
struct ProductTradeTime
{
	ProductTradingTimeType TradeTimeInterval;
	int BeginTime;
	int EndTime;

};


enum BarType{
	Second,
	Minute,
	Hour,
	Day,
	Week,	//not supported yet;	
	Month	//not supported yet;
};

 extern std::map<std::string, std::map<int, ProductTradeTime> > TodayProductTradeTime;
//0900,四位时分格式转换为秒
int TimeIntervalToSeconds(const char* time);

int		ToSeconds(const char * pszTime);
int TimeToInt(const char* time);

long GetNextDate(long date);

int GetLocalTimeSeconds();
int GetLocalTime();
int GetLocalDate();
char *	ToTimeString(int seconds,char * pszTime);

const char * GetExchange(const char *pszInstrumentID);



void ReadTradeTimeMap();
void ReadProductID(std::string& instStr, std::vector<std::string>& instVec);
void ReadTimeGap(std::string& timeStr, std::map<int, ProductTradeTime>& timeMap);

void ClearTradeTimeMap();

class CKBar {
//attributes;
public:
	char	m_sInstrumentID[30]; //合约编号
	char	m_sTradingDay[16];	//本bar开始时间的交易日20210120
	char	m_sActionDay[16];	//本bar开始时的实际日期20210120
	char	m_sBeginTime[12];	//本bar开始时间09：00：00
	char	m_sEndTime[12];		//本bar结束时间09：01：00

	double	m_fOpen;	//开盘价;
	double	m_fHighest;	//最高价
	double	m_fLowest;	//最低价
	double	m_fClose;	//收盘价
	double	m_fAverage;	//均价
	long	m_nVolume;	//成交量
	double	m_fAmmount; //成交金额,
	long	m_nPosition;	//持仓量
	long	m_nPositionDiff;//持仓量变化;
	//生成K线时需要用到
	long	m_nStatus;//Bar状态;0-未关闭,1关闭;-1未初始化

public:

	//internal used members;
	long	m_nLatestVolume;
	double	m_fLatestAmmount;
	long	m_nBeginVolume;
	double	m_fBeginAmmount;
	long	m_nBeginPosition;
	long	m_nMultiply;
	int		m_nEndTimeDataCount;
	int		m_nNextBeginSeconds;

	void *  m_pSeries;

	//add:2011-6-29
	double m_fDayHighest;
	double m_fDayLowest;

	BarType	m_BarType;
	int		m_nInterval;
	int		m_nCurrentSecond;
	int		m_nStartSecond;
	int		m_nEndSecond;
	int		m_nIndex;

	const char * GetSeriesName();

//operations;构造函数，初始化k线数据
	CKBar(void){
		m_nMultiply = 1;
		m_fLatestAmmount = 0;
		m_nLatestVolume = 0;
		m_nStatus = -1;
		m_BarType = Minute;
		m_nInterval = 1;
		m_fOpen = 0;
		m_fHighest = 0;
		m_fLowest = 0;
		m_fClose = 0;
		m_fAverage = 0;
		m_nVolume = 0;
		m_fAmmount = 0;
		m_nStartSecond = 0;
		m_nEndSecond = 0;
		m_sTradingDay[0] = '\0';
		m_sActionDay[0] = '\0';
		m_sBeginTime[0] = '\0';
		m_sEndTime[0] = '\0';
		m_nBeginPosition = 0;
		m_nBeginVolume =0;
		m_fBeginAmmount =0;
		m_nPositionDiff = 0;

		m_nEndTimeDataCount = 0;
		m_sInstrumentID[0] = '\0';
		m_nNextBeginSeconds = 0;

		//add: 2011-6-29
		m_fDayHighest = -999999;
		m_fDayLowest = 999999;
	}

	//判断是否当天的第一个k线
	bool IsFirstBar(){

		std::string strPrID=GetProductID(m_sInstrumentID);
		auto it = TodayProductTradeTime.find(strPrID);
		if (it != TodayProductTradeTime.end())
		{
			int bTime=(it->second.begin())->second.BeginTime;
			if (m_nStartSecond <= bTime)
				return true;
		}
		//if(m_sInstrumentID[0]=='I' && m_sInstrumentID[1]=='F'){	//中金所时间调整;
		//	if(m_nStartSecond <= 9*3600+30*60){
		//		return true;
		//	}
		//}else{
		//	if(m_nStartSecond <= 9*3600){
		//		return true;
		//	}
		//}
		return false;
	}

	//判断是否当天的最后一个k线
	bool IsLastBar(){
		//if(m_sInstrumentID[0]=='I' && m_sInstrumentID[1]=='F'){	//中金所时间调整;
		//	//if(m_nEndSecond >= 15*3600+15*60){
		//	//	return true;
		//	//}
		//}else{
		//	//if(m_nEndSecond >= 15*3600){
		//	//	return true;
		//	//}
		//}
		std::string strPrID = GetProductID(m_sInstrumentID);
		auto it = TodayProductTradeTime.find(strPrID);
		if (it != TodayProductTradeTime.end())
		{
			int eTime = (it->second.rbegin())->second.EndTime;
			if (m_nEndSecond >= eTime)
				return true;
		}
		/*if(m_nEndSecond >= 15*3600){
				return true;
			}*/
		return false;
	}
	
	//生成新的k线时，用老的k线的公共部分赋值给新的k线
	CKBar * Clone(CThostFtdcDepthMarketDataField *pBeginTickData,bool asOpen){
		char buf[300];

		CKBar * pBar = new CKBar();

		strcpy(pBar->m_sInstrumentID,m_sInstrumentID);

		pBar->m_nBeginVolume = this->m_nLatestVolume;
		pBar->m_fBeginAmmount = this->m_fLatestAmmount;
		pBar->m_nBeginPosition = this->m_nPosition;

		pBar->m_nPosition = pBeginTickData->OpenInterest;

		pBar->m_BarType = this->m_BarType;
		pBar->m_nInterval = this->m_nInterval;
		pBar->m_nIndex = m_nIndex+1;

		pBar->m_fOpen = pBeginTickData->LastPrice;
		pBar->m_fClose = pBeginTickData->LastPrice;
		pBar->m_fHighest = pBeginTickData->LastPrice;
		pBar->m_fLowest = pBeginTickData->LastPrice;

		strcpy(pBar->m_sTradingDay,pBeginTickData->TradingDay);
		SetActionDay(pBeginTickData);
		pBar->m_pSeries = this->m_pSeries;
		//m_nNextBeginSeconds = (24 + 11) * 3600 + 1800;
		pBar->SetStartSecond(ToTimeString(m_nNextBeginSeconds,buf));
		pBar->m_nMultiply = this->m_nMultiply;
		pBar->SetEndSecond();

		if(asOpen){
			pBar->m_nStatus = 0;
		}else{
			pBar->m_nStatus = -1;
		}

		//current bar
		if(m_nStatus == 2){
			pBar->m_nStatus = -1;
		}

		//add 2011-6-29
		pBar->m_fDayHighest = pBeginTickData->HighestPrice;
		pBar->m_fDayLowest = pBeginTickData->LowestPrice;

		return pBar;
	}

	//检查这根k线是否关闭
	bool CheckIfClosed(CThostFtdcDepthMarketDataField *pLatest,int seconds=0,bool *asOpen=NULL){
		bool closed = false;
		if(seconds == 0){
			seconds = ToSeconds(pLatest->UpdateTime);
		}

		int timeDiff = seconds - m_nEndSecond+1;

		if(timeDiff <= 0){//在本Bar结束时间之前;
			if(m_nStatus == -1){//本bar的初始数据还没有设置;
				if(strlen(m_sBeginTime)<1){
					strcpy(m_sBeginTime,pLatest->UpdateTime);
				}
				m_nLatestVolume = m_nBeginVolume = pLatest->Volume;
				m_fLatestAmmount = m_fBeginAmmount = pLatest->Turnover;
				m_nPosition = pLatest->OpenInterest;
				m_nBeginPosition = pLatest->OpenInterest;

				m_nStatus = 0;

				m_fOpen = pLatest->LastPrice;
				m_fClose = pLatest->LastPrice;
				m_fHighest = pLatest->LastPrice;
				m_fLowest = pLatest->LastPrice;

				m_fDayHighest = pLatest->HighestPrice;
				m_fDayLowest = pLatest->LowestPrice;
			}
			//已经初始化，则更新bar
			else{
				m_fLatestAmmount = pLatest->Turnover;
				m_nLatestVolume = pLatest->Volume;
				m_nPosition = pLatest->OpenInterest;

				m_nVolume = m_nLatestVolume - m_nBeginVolume;
				m_fAmmount = m_fLatestAmmount - m_fBeginAmmount;
				m_nPositionDiff = m_nPosition-m_nBeginPosition;

				//update:2011-6-29;
				if(pLatest->HighestPrice > m_fDayHighest){
					m_fDayHighest = pLatest->HighestPrice;
					m_fHighest = m_fDayHighest;
				}else{
					if(pLatest->LastPrice > m_fHighest){
						m_fHighest = pLatest->LastPrice;
					}
				}
				if(pLatest->LowestPrice < m_fDayLowest){
					m_fDayLowest = pLatest->LowestPrice;
					m_fLowest = m_fDayLowest;
				}else{
					if(pLatest->LastPrice < m_fLowest){
						m_fLowest = pLatest->LastPrice;
					}
				}

				m_fClose = pLatest->LastPrice;

				
				if(strcmp(pLatest->ExchangeID,"CZCE")!=0 && m_nMultiply !=0){
					m_fAmmount = m_fAmmount/ m_nMultiply;
				}

				if(m_nVolume > 0){
					m_fAverage = m_fAmmount/m_nVolume;
				}
			}
			//在结束的秒上面
		if (timeDiff == 0) {
			m_nEndTimeDataCount++;
			if (m_nEndTimeDataCount >= 2) {	//本Bar结束时间点上已经有两个数据, 1秒2个数据;
				m_nEndTimeDataCount = 0;
				m_nStatus = 1; //结束bar
				closed = true;

				long seconds;
				if (m_BarType == Second) {
					seconds = m_nInterval;
				}
				else if (m_BarType == Minute) {
					seconds = m_nInterval * 60;
				}
				else if (m_BarType == Hour) {
					seconds = m_nInterval * 3600;
				}
				else if (m_BarType == Day) {
					//seconds = 8*3600+15*60;
					seconds = 0;
				}
				else {
					throw "period type error";
				}

				m_nNextBeginSeconds = m_nStartSecond + seconds;
				return closed;
			}
		}
		
		}
		

		//>0, already closed; 已经超过本Bar结束时间;
		else{ 
			

			if(m_nStatus == -1){	//本周期内无成交数据;
				m_fLatestAmmount = pLatest->Turnover;
				m_nLatestVolume = pLatest->Volume;
				m_nPosition = pLatest->OpenInterest;

				if(pLatest->LastPrice > m_fHighest){
					m_fHighest = pLatest->LastPrice;
				}
				if(pLatest->LastPrice < m_fLowest){
					m_fLowest = pLatest->LastPrice;
				}
				m_fClose = pLatest->LastPrice;

				m_nVolume = m_nLatestVolume - m_nBeginVolume;
				m_fAmmount = m_fLatestAmmount - m_fBeginAmmount;
				m_nPositionDiff = m_nPosition-m_nBeginPosition;

				if (strcmp(pLatest->ExchangeID, "CZCE") != 0 && m_nMultiply != 0) {
					m_fAmmount = m_fAmmount / m_nMultiply;
				}

				if (m_nVolume > 0) {
					m_fAverage = m_fAmmount / m_nVolume;
				}
			}



			if(asOpen != NULL){
				*asOpen = true;
			}
			m_nStatus = 1;

			long seconds;
			if(m_BarType == Second){
				seconds = m_nInterval;
			}else if(m_BarType == Minute){
				seconds = m_nInterval*60;
			}else if(m_BarType == Hour){
				seconds = m_nInterval*3600;
			}else if(m_BarType == Day){
				//seconds = 8*3600+15*60;
				seconds = 0;
			}else{
				throw "period type error";
			}

			m_nNextBeginSeconds = m_nStartSecond+seconds;

			closed = true;
		}

		return closed;
	}

	//设置合约乘数
	void SetMultiply(long nMultiply){
		m_nMultiply = nMultiply;
	}

	//set the start second of a bar;
	void SetStartSecond(const char * pszNextBeginTime){
		bool NeedAdjust = true;
		char cBuf[64];

		int curSeconds = ToSeconds(pszNextBeginTime);

		std::string strPrID = GetProductID(m_sInstrumentID);
		auto it = TodayProductTradeTime.find(strPrID);
		if (it != TodayProductTradeTime.end())
		{
			int lastEndTime = 0;
			for (auto iter = it->second.begin(); iter != it->second.end(); iter++)
			{
				int bTime = iter->second.BeginTime;
				if (lastEndTime == 0)
				{
					if (curSeconds <= bTime)
					{
						m_nStartSecond = bTime;
						NeedAdjust = false;
						break;
					}	
				}
				else
				{
					if (curSeconds >= lastEndTime && curSeconds <= bTime)
					{
						m_nStartSecond = bTime;
						NeedAdjust = false;
						break;
					}
				}

				
				lastEndTime = iter->second.EndTime;

			}
			

			if (NeedAdjust || m_BarType == Day) {
				if (m_BarType == Second) {
					m_nStartSecond = curSeconds / m_nInterval * m_nInterval;
				}
				else if (m_BarType == Minute) {
					m_nStartSecond = curSeconds / (m_nInterval * 60) * (m_nInterval * 60);
				}
				else if (m_BarType == Hour) {
					m_nStartSecond = curSeconds / (m_nInterval * 3600) * (m_nInterval * 3600);
				}
				else if (m_BarType == Day) {
					m_nStartSecond = it->second.begin()->second.BeginTime;
					/*if(isCFFEX){
						m_nStartSecond = 9*3600+30*60;
					}else{
						m_nStartSecond = 9*3600;
					}*/
				}
				else {
					throw "period type error";
				}
			}

			strcpy(m_sBeginTime, ToTimeString(m_nStartSecond, cBuf));

		}

		//bool isCFFEX = false;

		//if(m_sInstrumentID[0]=='I' && m_sInstrumentID[1]=='F'){	//中金所时间调整;
		//	isCFFEX = true;
		//}


		//if(isCFFEX){
		//	if(curSeconds<9*3600+30*60){
		//		m_nStartSecond = 9*3600+30*60;
		//		NeedAdjust = false;
		//	}else{
		//		if(curSeconds >=(11*3600+30*60) && curSeconds<=13*3600){
		//			m_nStartSecond = 13*3600;
		//			NeedAdjust = false;
		//		}
		//	}
		//}else{
		//	if(curSeconds<9*3600){
		//		m_nStartSecond = 9*3600;
		//		NeedAdjust = false;
		//	}else{
		//		if(curSeconds >=(10*3600+15*60) && curSeconds<=10*3600+30*60){
		//			m_nStartSecond = 10*3600+30*60;
		//			NeedAdjust = false;
		//		}else if(curSeconds >=(11*3600+30*60) && curSeconds<=13*3600){
		//			m_nStartSecond = 13*3600+30*60;
		//			NeedAdjust = false;
		//		}
		//	}
		//}

		
	}

	//set the end second of this bar;
	void SetEndSecond(){
		char buf[30];

		bool needToAdjust = true;

		std::string strPrID = GetProductID(m_sInstrumentID);
		auto it = TodayProductTradeTime.find(strPrID);
		if (it != TodayProductTradeTime.end())
		{
			int seconds;
			if (m_BarType == Second) {
				seconds = m_nInterval;
			}
			else if (m_BarType == Minute) {
				seconds = m_nInterval * 60;
			}
			else if (m_BarType == Hour) {
				seconds = m_nInterval * 3600;
			}
			else if (m_BarType == Day) {
				seconds = it->second.rbegin()->second.EndTime - it->second.begin()->second.BeginTime;
			}
			else {
				throw "period type error";
			}

			m_nEndSecond = m_nStartSecond + seconds;

			int lastEndTime = 0;
			for (auto iter = it->second.begin(); iter != it->second.end(); iter++)
			{
				int bTime = iter->second.BeginTime;
				if (lastEndTime != 0)
				{
					if (m_nEndSecond >= lastEndTime && m_nEndSecond<=bTime)
					{
						m_nEndSecond = lastEndTime;
						needToAdjust = false;
						break;
					}
				}

				lastEndTime = iter->second.EndTime;

			}


			//if (m_sInstrumentID[0] == 'I' && m_sInstrumentID[1] == 'F') {	//中金所时间调整;
			//	if (m_nEndSecond >= 11 * 3600 + 30 * 60 && m_nEndSecond <= 13 * 3600) {
			//		m_nEndSecond = 11 * 3600 + 30 * 60 + 1;
			//		needToAdjust = false;
			//	}
			//}
			//else {
			//	if (m_nEndSecond > 10 * 3600 + 15 * 60 && m_nEndSecond <= 10 * 3600 + 30 * 60) {	//上午小节休息时间调整
			//		m_nEndSecond = 10 * 3600 + 15 * 60;
			//		needToAdjust = false;
			//	}
			//	else if (m_nEndSecond >= 11 * 3600 + 30 * 60 + 3 && m_nEndSecond <= 13 * 3600 + 30 * 60) {	//午间休息时间调整;
			//		m_nEndSecond = 11 * 3600 + 30 * 60;
			//		needToAdjust = false;
			//	}
			//}

			//如果是需要调整时间;
			if (needToAdjust) {
				int left = m_nEndSecond % seconds;
				if (left != 0) {//整数秒;
					m_nEndSecond -= left;
				}
			}

			strcpy(m_sEndTime, ToTimeString(m_nEndSecond, buf));

		}
		

		
	}
	
	//获取合约的产品代码，比如IF2102，它的产品代码就是IF，交割的时候，交易时间可能会不一样
	std::string GetProductID(const char* pInst)
	{
		string str;
		str = m_sInstrumentID[0];
		if (!isdigit(m_sInstrumentID[1]))
			str += m_sInstrumentID[1];
		return str;
	}

	//设置k线开始时间所属的实际日期，深度行情回报里面也有一个ActionDay的字段，中金所、上期所、能源所、郑州的ActionDay是实际日期，大商所则不同
	void SetActionDay(CThostFtdcDepthMarketDataField* pLatest)
	{
		int i_sec = ToSeconds(pLatest->UpdateTime);
		if (i_sec < 24 * 3600)
			strcpy(m_sActionDay, g_cPreTradingDay);
		else if (i_sec >= 24 * 3600 && i_sec <= 30 * 3600)
		{
			/*long ltd=GetNextDate(atol(g_cPreTradingDay));
			sprintf(m_sActionDay, "%ld", ltd);*/
			strcpy(m_sActionDay, g_cNextTradeDate);
		}
		else
		{
			strcpy(m_sActionDay, pLatest->TradingDay);
		}
			 
			
	}

	void ShowBar()
	{
		cerr << endl << endl;
		cerr << "合约：" << m_sInstrumentID
			<< " ,索引：" << m_nIndex
			<< " ,日期：" << m_sActionDay
			<< " ,开始时间：" << m_sBeginTime
			<< " ,结束时间：" << m_sEndTime
			<< " ,开盘价：" << m_fOpen
			<< " ,最高价：" << m_fHighest
			<< " ,最低价：" << m_fLowest
			<< " ,收盘价：" << m_fClose
			<< " ,成交量：" << m_nVolume
			<< " ,持仓量：" << m_nPosition;
		cerr << endl << endl;
	}

	int SaveKBar(std::string strPath)
	{
		char filePath[100] = { '\0' };
		//生成文件的路径
		sprintf_s(filePath, sizeof(filePath), "%s.csv", strPath.c_str());
		std::ofstream outFile;
		outFile.open(filePath, ios::app);//文件追加写入
		//合约代码, 业务日期, 交易日, 开始时间, 结束时间, 开盘价, 最高价, 最低价, 收盘价, 均价, 成交量, 持仓量, 持仓量变化
		outFile << this->m_sInstrumentID << ","
			<< this->m_sActionDay << ","
			<< this->m_sTradingDay << ","
			<< this->m_sBeginTime << ","
			<< this->m_sEndTime << ","
			<< this->m_fOpen << ","
			<< this->m_fHighest << ","
			<< this->m_fLowest << ","
			<< this->m_fClose << ","
			<< this->m_fAverage << ","
			<< this->m_nVolume << ","
			<< this->m_nPosition << ","
			<< this->m_nPositionDiff
			<< endl;

		outFile.close();
		return 0;
	}
};

class CKBarSeries  {

private:
	//CKBar的二级指针
	CKBar		**m_ppData;
	//计数器，已经生成了多少k线
	int			m_nCount;
	//数组的指针
	double		*m_pClosePrice;
	double		*m_pOpenPrice;
	double		*m_pHighPrice;
	double		*m_pLowPrice;
	double		*m_pVolume;
	double		*m_pPosition;
	double		*m_pAvgPrice;
	double		*m_pAmmount;
	double		*m_pPositionDiff;


public:
	//名称，IF2102_15_Minute
	char	m_sDisplayName[128];
	//合约名称
	char	m_sName[32];
	//K线类型
	BarType	m_BarType;
	//K线周期数
	int		m_nInterval;
	//合约乘数
	int		m_nMultiply;
	//总的k线数量
	int		m_nTotal;

private:
	void Lock(){
	}

	void Unlock(){
	}

public:
	//构造函数
	CKBarSeries(const char * pszName,BarType barType,int nInterval,int nMultiply,int nTotal){
		strcpy(m_sName,pszName);
		m_nInterval = nInterval;
		m_BarType = barType;
		m_nCount = 0;
		m_nTotal = nTotal;
		m_nMultiply = nMultiply;
		m_ppData = new CKBar*[nTotal];
		m_pClosePrice = new double[nTotal];
		m_pOpenPrice = new double[nTotal];
		m_pHighPrice = new double[nTotal];
		m_pLowPrice = new double[nTotal];
		m_pVolume = new double[nTotal];
		m_pPosition = new double[nTotal];
		m_pAvgPrice = new double[nTotal];
		m_pAmmount = new double[nTotal];
		m_pPositionDiff = new double[nTotal];

		m_sDisplayName[0]='\0';
	}
	//析构函数
	~CKBarSeries(){
		for(int i=0;i<m_nCount;i++){
			delete m_ppData[i];
		}

		delete[] m_ppData;
		delete[] m_pVolume;
		delete[] m_pClosePrice;
		delete[] m_pPosition;
		delete[] m_pHighPrice;
		delete[] m_pOpenPrice;
		delete[] m_pLowPrice;
		delete[] m_pAvgPrice;
		delete[] m_pAmmount;
		delete[] m_pPositionDiff;
	}


	const char *GetName(){
		return m_sName;
	}

	virtual  const char *GetInstrumentID(){
		return m_sName;
	}

	virtual BarType GetType(){
		return m_BarType;
	}

	virtual int		GetPeriod(){
		return m_nInterval;
	}
	//生成一根新的k线
	CKBar *NewBar(){
		CKBar *pBar = new CKBar();
		pBar->m_pSeries = this;
		pBar->m_BarType = this->m_BarType;
		pBar->m_nInterval = this->m_nInterval;
		strcpy(pBar->m_sInstrumentID,this->m_sName);
		pBar->m_nMultiply = this->m_nMultiply;

		return pBar;
	}
	void ReadKbar(const char * barStr, CKBar &bar);
	//tick数据计算，如果k线关闭，则返回k线，否则为空
	CKBar * AddTickData(CThostFtdcDepthMarketDataField *pData){
		CKBar * pClosedBar = NULL;

		if(m_nCount >= m_nTotal){//已经完成;
			return NULL;
		}

		if(m_nCount < 1 || m_ppData[m_nCount-1]->m_nStatus==1){ //create the first bar,或者前面的Bar已经结束;
			if(m_nCount>0){
				if(m_ppData[m_nCount-1]->IsLastBar()){	//last bar closed.
					return NULL;
				}
			}

			//first new bar;
			CKBar * pBar = NewBar();
			strcpy(pBar->m_sTradingDay,pData->TradingDay);
			pBar->SetActionDay(pData);
			pBar->SetStartSecond(pData->UpdateTime);
			pBar->SetEndSecond();

			//update 2011-6-30;
			if(pBar->IsFirstBar()){
				pBar->m_nBeginVolume = 0;
				pBar->m_fBeginAmmount = 0;
				pBar->m_nBeginPosition = pData->PreOpenInterest;
			}else{
				pBar->m_nBeginVolume = pData->Volume;
				pBar->m_fBeginAmmount = pData->Turnover;
				pBar->m_nBeginPosition = pData->OpenInterest;
			}

			pBar->m_nPosition = pData->OpenInterest;

			pBar->m_fOpen = pData->LastPrice;
			pBar->m_fClose = pData->LastPrice;
			pBar->m_fHighest = pData->LastPrice;
			pBar->m_fLowest = pData->LastPrice;

			pBar->m_nStatus = 0;
			if(m_nCount<1){
				pBar->m_nIndex = 0;
			}else{
				pBar->m_nIndex = m_ppData[m_nCount-1]->m_nIndex+1;
			}

			//strcpy(pBar->m_sBeginTime,pData->UpdateTime);

			m_ppData[m_nCount++] = pBar;

			//2011-6-17 begin;
			m_pOpenPrice[m_nCount-1]=pBar->m_fOpen;
			m_pHighPrice[m_nCount-1]=pBar->m_fHighest;
			m_pLowPrice[m_nCount-1]=pBar->m_fLowest;
			m_pClosePrice[m_nCount-1]=pBar->m_fClose;
			m_pVolume[m_nCount-1]= 0 ;
			m_pPosition[m_nCount-1]=pBar->m_nPosition;
			m_pPositionDiff[m_nCount-1]=0;
			m_pAvgPrice[m_nCount-1]=0;
			m_pAmmount[m_nCount-1]=0;
			//2011-6-17 end;

			//add 2011-6-29
			pBar->m_fDayHighest = pData->HighestPrice;
			pBar->m_fDayLowest = pData->LowestPrice;

			return NULL;
		}
		//当前bar还没有关闭
		else{
			CKBar * pCurrentBar = m_ppData[m_nCount-1];

			bool asOpen=false;

			if(pCurrentBar->CheckIfClosed(pData,0,&asOpen)){//if closed;
				//file the close price double array;
				m_pOpenPrice[m_nCount-1]=pCurrentBar->m_fOpen;
				m_pHighPrice[m_nCount-1]=pCurrentBar->m_fHighest;
				m_pLowPrice[m_nCount-1]=pCurrentBar->m_fLowest;
				m_pClosePrice[m_nCount-1]=pCurrentBar->m_fClose;
				m_pVolume[m_nCount-1]=pCurrentBar->m_nVolume;
				m_pPosition[m_nCount-1]=pCurrentBar->m_nPosition;
				m_pPositionDiff[m_nCount-1]=pCurrentBar->m_nPositionDiff;
				m_pAvgPrice[m_nCount-1]=pCurrentBar->m_fAverage;
				m_pAmmount[m_nCount-1]=pCurrentBar->m_fAmmount;

				if(!pCurrentBar->IsLastBar()){  //if is not the last bar, then create a new bar;
					CKBar * pNewBar = pCurrentBar->Clone(pData,asOpen);

					m_ppData[m_nCount++] = pNewBar;

					//2011-6-17 begin;
					m_pOpenPrice[m_nCount-1]=pNewBar->m_fOpen;
					m_pHighPrice[m_nCount-1]=pNewBar->m_fHighest;
					m_pLowPrice[m_nCount-1]=pNewBar->m_fLowest;
					m_pClosePrice[m_nCount-1]=pNewBar->m_fClose;
					m_pVolume[m_nCount-1]= 0 ;
					m_pPosition[m_nCount-1]=pNewBar->m_nPosition;

					m_pPositionDiff[m_nCount-1]=0;
					m_pAvgPrice[m_nCount-1]=0;
					m_pAmmount[m_nCount-1]=0;
					//2011-6-17 end;
				}
				pClosedBar = pCurrentBar;
			}
			//没有关闭，则更新数据
			else{
				//2011-6-17 begin;
				m_pHighPrice[m_nCount-1]=pCurrentBar->m_fHighest;
				m_pLowPrice[m_nCount-1]=pCurrentBar->m_fLowest;
				m_pClosePrice[m_nCount-1]=pCurrentBar->m_fClose;
				m_pVolume[m_nCount-1]=pCurrentBar->m_nVolume;
				m_pPosition[m_nCount-1]=pCurrentBar->m_nPosition;
				m_pPositionDiff[m_nCount-1]=pCurrentBar->m_nPositionDiff;
				m_pAvgPrice[m_nCount-1]=pCurrentBar->m_fAverage;
				m_pAmmount[m_nCount-1]=pCurrentBar->m_fAmmount;
				//2011-6-17 end;
			}
		}

		return pClosedBar;
	}
	
	//针对冷月合约，可能几分钟都没有成交数据，根据时间生成k线
	CKBar * UpdateByTime(CThostFtdcDepthMarketDataField *pData,long seconds,bool useSeconds=false){
		char cBuf[16];

		CKBar * pClosedBar = NULL;

		if(m_nCount >= m_nTotal){//已经完成;
			return NULL;
		}

		if(m_nCount < 1 || m_ppData[m_nCount-1]->m_nStatus==1){ //create the first bar,或者前面的Bar已经结束;只在尚无bar或者,bar已经结束的情况下才会调用;
			if(m_nCount>0){
				if(m_ppData[m_nCount-1]->IsLastBar()){	//last bar closed.
					return NULL;
				}
			}

			//first new bar;
			CKBar * pBar = NewBar();

			strcpy(pBar->m_sTradingDay,pData->TradingDay);
			if(useSeconds){
				pBar->SetStartSecond(ToTimeString(seconds,cBuf));
			}else{
				pBar->SetStartSecond(pData->UpdateTime);
			}

			pBar->SetEndSecond();
			pBar->SetActionDay(pData);
			pBar->m_nBeginVolume = pData->Volume;
			pBar->m_fBeginAmmount = pData->Turnover;
			pBar->m_nPosition = pData->OpenInterest;
			pBar->m_nBeginPosition = pData->OpenInterest;

			pBar->m_fOpen = pData->LastPrice;
			pBar->m_fClose = pData->LastPrice;
			pBar->m_fHighest = pData->LastPrice;
			pBar->m_fLowest = pData->LastPrice;

			pBar->m_nStatus = -1;
			if(m_nCount<1){
				pBar->m_nIndex = 0;
			}else{
				pBar->m_nIndex = m_ppData[m_nCount-1]->m_nIndex+1;
			}

			m_ppData[m_nCount++] = pBar;

			//2011-6-17 begin;
			m_pOpenPrice[m_nCount-1]=pBar->m_fOpen;
			m_pHighPrice[m_nCount-1]=pBar->m_fHighest;
			m_pLowPrice[m_nCount-1]=pBar->m_fLowest;
			m_pClosePrice[m_nCount-1]=pBar->m_fClose;
			m_pVolume[m_nCount-1]= 0 ;
			m_pPosition[m_nCount-1]=pBar->m_nPosition;

			m_pPositionDiff[m_nCount-1]=0;
			m_pAvgPrice[m_nCount-1]=0;
			m_pAmmount[m_nCount-1]=0;
			//2011-6-17 end;

			return NULL;
		}else{
			CKBar * pCurrentBar = m_ppData[m_nCount-1];
			bool asOpen = false;

			if(pCurrentBar->CheckIfClosed(pData,seconds,&asOpen)){//if closed;
				//file the close price double array;
				m_pOpenPrice[m_nCount-1]=pCurrentBar->m_fOpen;
				m_pHighPrice[m_nCount-1]=pCurrentBar->m_fHighest;
				m_pLowPrice[m_nCount-1]=pCurrentBar->m_fLowest;
				m_pClosePrice[m_nCount-1]=pCurrentBar->m_fClose;
				m_pVolume[m_nCount-1]=pCurrentBar->m_nVolume;
				m_pPosition[m_nCount-1]=pCurrentBar->m_nPosition;
				m_pPositionDiff[m_nCount-1]=pCurrentBar->m_nPositionDiff;
				m_pAvgPrice[m_nCount-1]=pCurrentBar->m_fAverage;
				m_pAmmount[m_nCount-1]=pCurrentBar->m_fAmmount;

				if(!pCurrentBar->IsLastBar()){
					CKBar * pNewBar = pCurrentBar->Clone(pData,asOpen);
					m_ppData[m_nCount++] = pNewBar;

					//2011-6-17 begin;
					m_pOpenPrice[m_nCount-1]=pNewBar->m_fOpen;
					m_pHighPrice[m_nCount-1]=pNewBar->m_fHighest;
					m_pLowPrice[m_nCount-1]=pNewBar->m_fLowest;
					m_pClosePrice[m_nCount-1]=pNewBar->m_fClose;
					m_pVolume[m_nCount-1]= 0 ;
					m_pPosition[m_nCount-1]=pNewBar->m_nPosition;

					m_pPositionDiff[m_nCount-1]=0;
					m_pAvgPrice[m_nCount-1]=0;
					m_pAmmount[m_nCount-1]=0;
					//2011-6-17 end;
				}

				pClosedBar = pCurrentBar;
			}else{	//更新到last bar中；
				//2011-6-17 begin;
				m_pHighPrice[m_nCount-1]=pCurrentBar->m_fHighest;
				m_pLowPrice[m_nCount-1]=pCurrentBar->m_fLowest;
				m_pClosePrice[m_nCount-1]=pCurrentBar->m_fClose;
				m_pVolume[m_nCount-1]=pCurrentBar->m_nVolume;
				m_pPosition[m_nCount-1]=pCurrentBar->m_nPosition;
				m_pPositionDiff[m_nCount-1]=pCurrentBar->m_nPositionDiff;
				m_pAvgPrice[m_nCount-1]=pCurrentBar->m_fAverage;
				m_pAmmount[m_nCount-1]=pCurrentBar->m_fAmmount;
				//2011-6-17 end;
			}
		}

		return pClosedBar;
	}

	//把这个k线添加到kbarseries
	void AddKBar(CKBar *pBar){
		m_ppData[m_nCount] = pBar;

		m_pOpenPrice[m_nCount]=pBar->m_fOpen;
		m_pHighPrice[m_nCount]=pBar->m_fHighest;
		m_pLowPrice[m_nCount]=pBar->m_fLowest;
		m_pClosePrice[m_nCount]=pBar->m_fClose;
		m_pVolume[m_nCount]=pBar->m_nVolume;
		m_pAmmount[m_nCount]=pBar->m_fAmmount;
		m_pAvgPrice[m_nCount]=pBar->m_fAverage;
		m_pPosition[m_nCount]=pBar->m_nPosition;
		m_pPositionDiff[m_nCount]=pBar->m_nPositionDiff;

		m_nCount++;
	}

	void GetClosePrice(double **ppData,int* pCount){
		*ppData = m_pClosePrice;
		*pCount = m_nCount;
	}

	void GetOpenPrice(double **ppData,int* pCount){
		*ppData = m_pOpenPrice;
		*pCount = m_nCount;
	}

	void GetHighPrice(double **ppData,int* pCount){
		*ppData = m_pHighPrice;
		*pCount = m_nCount;
	}

	void GetLowPrice(double **ppData,int* pCount){
		*ppData = m_pLowPrice;
		*pCount = m_nCount;
	}

	void GetAvgPrice(double **ppData,int* pCount){
		*ppData = m_pAvgPrice;
		*pCount = m_nCount;
	}

	void GetAmmount(double **ppData,int* pCount){
		*ppData = m_pAmmount;
		*pCount = m_nCount;
	}

	void GetVolume(double **ppData,int* pCount){
		*ppData = m_pVolume;
		*pCount = m_nCount;
	}

	void GetPosition(double **ppData,int* pCount){
		*ppData = m_pPosition;
		*pCount = m_nCount;
	}

	void GetPositionDiff(double **ppData,int* pCount){
		*ppData = m_pPositionDiff;
		*pCount = m_nCount;
	}
	void GetCount(int* pCount) {
		*pCount = m_nCount;
	}
	virtual void GetPricesAndVolume(double **ppOpenPrices,double **ppHighPrices,double **ppLowPrices,double **ppClosePrices,double **ppVolume,int* pCount){
		*ppOpenPrices = m_pOpenPrice;
		*ppHighPrices = m_pHighPrice;
		*ppLowPrices = m_pLowPrice;
		*ppClosePrices = m_pClosePrice;
		*ppVolume = m_pVolume;

		*pCount = m_nCount;
	}

	virtual void GetBars(CKBar **ppBars,int * pCount){
		*pCount = m_nCount;

		for(int i=0;i<*pCount;i++){
			ppBars[i] = m_ppData[i];
		}
	}

	virtual CKBar * GetTopBar(){
		return m_ppData[m_nCount-1];
	}
};

//This class is exported from the KData.dll
class CKDataCollection {
private:
	//多个品种多个周期的k线数据
	map<string,map<string,CKBarSeries*> *> m_Data;
	//多个品种深度行情数据快照，保存最新的行情快照
	map<string,CThostFtdcDepthMarketDataField*> m_MarketData;

public:
	CThostFtdcDepthMarketDataField * GetMarketData(const char *pszCode);

	CKDataCollection(void);

	~CKDataCollection(){
		Clear();
	}

	virtual void Clear(){
		map<string,map<string,CKBarSeries*> *>::iterator it = m_Data.begin();
		map<string,map<string,CKBarSeries*> *>::iterator end = m_Data.end();
		while(it != end){
			map<string,CKBarSeries*> * pMap = it->second;
			map<string,CKBarSeries*>::iterator it2 = pMap->begin();
			map<string,CKBarSeries*>::iterator end2 = pMap->end();
			while(it2 != end2){
				delete it2->second;
				it2++;
			}
			pMap->clear();
			delete it->second;
			it++;
		}
		m_Data.clear();


		map<string,CThostFtdcDepthMarketDataField*>::iterator itm = m_MarketData.begin();
		map<string,CThostFtdcDepthMarketDataField*>::iterator ite = m_MarketData.end();
		while(itm != ite){
			delete itm->second;
			itm++;
		}
		m_MarketData.clear();
	}

	//生成一个kbarseries
	CKBarSeries * CreateSeries(const char * pszName,BarType barType,int nInterval,int volumeMultiply,int nTotal){
		char buf[30];

		CKBarSeries *pBS =  new CKBarSeries(pszName,barType,nInterval,volumeMultiply,nTotal);

		char cName[60];

		strcpy(cName,pszName);
		strcat(cName,"_");
		strcat(cName,_itoa(nInterval,buf,10));
		switch(barType){
		case Second:
			strcat(cName,"_second");
			break;
		case Minute:
			strcat(cName,"_minute");
			break;
		case Hour:
			strcat(cName,"_hour");
			break;
		case Day:
			strcat(cName,"_day");
			break;
		case Week:
			strcat(cName,"_week");
			break;
		case Month:
			strcat(cName,"_month");
			break;
		}

		map<string,CKBarSeries*> * pMap=NULL;
		map<string,map<string,CKBarSeries*> *>::iterator it = m_Data.find(pszName);
		if(it != m_Data.end()){
			pMap = it->second;
		}else{
			pMap = new map<string,CKBarSeries*>();
			m_Data.insert(pair<string,map<string,CKBarSeries*>*>(pszName,pMap));
		}
		//au2106_1_minute
		map<string,CKBarSeries*>::iterator its = pMap->find(cName);
		//没有找到
		if(its == pMap->end()){
			pMap->insert(pair<string,CKBarSeries*>(cName,pBS));
			strcpy(pBS->m_sDisplayName,cName);
			
		}else{
			delete pBS;
			return its->second;
		}
		its = pMap->find(cName);
		//找到了该k线序列
		if (its != pMap->end())
		{
			//au2106_1_minute  its->first
			ReadKBarSeries("E:/database/Kbar/"+its->first,its->second);
		}
		return pBS;
	}

	/**
	 *释放一个Bars系列;
	 */
	virtual void RemoveSeries(CKBarSeries *pBarSeries){
		map<string,CKBarSeries*> * pMap=NULL;
		map<string,map<string,CKBarSeries*> *>::iterator it = m_Data.find(pBarSeries->GetInstrumentID());
		if(it != m_Data.end()){
			pMap = it->second;
		}else{
			return;
		}

		pMap->erase(((CKBarSeries *)pBarSeries)->m_sDisplayName);
		delete (CKBarSeries *)pBarSeries;
	}

	const CKBarSeries * GetSeries(const char * pszName,BarType barType,int nInterval){
		char buf[60];

		map<string,CKBarSeries*> * pMap;
		map<string,map<string,CKBarSeries*> *>::iterator it = m_Data.find(pszName);
		if(it != m_Data.end()){
			pMap = it->second;

			char cName[60] = { '\0' };
			strcat(cName, pszName);
			strcat(cName, "_");
			strcat(cName,_itoa(nInterval,buf,10));
			switch(barType){
			case Second:
				strcat(cName,"_second");
				break;
			case Minute:
				strcat(cName,"_minute");
				break;
			case Hour:
				strcat(cName,"_hour");
				break;
			case Day:
				strcat(cName,"_day");
				break;
			case Week:
				strcat(cName,"_week");
				break;
			case Month:
				strcat(cName,"_month");
				break;
			}

			map<string,CKBarSeries*>::iterator it2 = pMap->find(cName);
			if(it2 != pMap->end()){
				return it2->second;
			}
		}

		return NULL;
	}

	void RemoveSeries(const char *pszDisplayName){
		int i=0;
		int len = strlen(pszDisplayName);
		char pszIns[30];

		pszIns[0]='\0';
		while(i<len){
			pszIns[i]=pszDisplayName[i];
			if(pszIns[i]=='_'){
				pszIns[i]='\0';
				break;
			}
			i++;
		}

		map<string,CKBarSeries*> * pMap=NULL;
		map<string,map<string,CKBarSeries*> *>::iterator it = m_Data.find(pszIns);
		if(it != m_Data.end()){
			pMap = it->second;
			map<string,CKBarSeries*>::iterator itk = pMap->find(pszDisplayName);
			if(itk != pMap->end()){
				delete itk->second;
				pMap->erase(pszDisplayName);
			}
		}
	}

	/**
	 *add a new tick data;计算相关bar,产生新的bar之后发给客户端;
	 */
	void AddTickData(CThostFtdcDepthMarketDataField* pTickData);

	/**
	 *按照时间来关闭Bar；
	 */
	void UpdateByTime(long seconds,const char *pszExchange){
		map<string,CKBarSeries*> * pMap;
		map<string,map<string,CKBarSeries*> *>::iterator it = m_Data.begin();
		while(it != m_Data.end()){
			const char *pszThisExchange = GetExchange(it->first.c_str());
			if(strcmp(pszThisExchange,pszExchange)==0){
				CThostFtdcDepthMarketDataField *pData = GetMarketData(it->first.c_str());
				if(pData != NULL){
					pMap = it->second;
					//更新具体的K线数据；
					map<string,CKBarSeries*>::iterator it2 = pMap->begin();
					map<string,CKBarSeries*>::iterator end2 = pMap->end();
					while(it2 != end2){
						CKBar * pBar = it2->second->UpdateByTime(pData,seconds,true);
						if(pBar != NULL){
							//bar closed;
							//todo process bar; 
							printf("%s close at %f on %s\n",pBar->m_sInstrumentID,pBar->m_fClose,pBar->m_sEndTime);
						}
						it2++;
					}
				}
			}
			it++;
		}
	}

	int ReadKBarSeries(std::string strPath, CKBarSeries* pBarSeries);
	
};

#endif