#pragma once
#include<string>
#include<vector>
#include"ThostFtdcUserApiDataType.h"
#include"ThostFtdcUserApiStruct.h"
//仓位信息结构体
struct position_field
{
	position_field()
	{
		instId = "";

		LongPosition = 0;
		TodayLongPosition = 0;
		YdLongPosition = 0;
		LongAvgEntryPrice = 0.0;//多仓的平均开仓价格
		LongAvgHoldingPrice = 0.0;
		LongCloseProfit = 0.0;
		LongOpenProfit = 0.0;
		LongPositionProfit = 0.0;

		ShortPosition = 0;
		TodayShortPosition = 0;
		YdShortPosition = 0;
		ShortAvgEntryPrice = 0.0;//空仓的平均开仓价格
		ShortAvgHoldingPrice = 0.0;
		ShortCloseProfit = 0.0;
		ShortOpenProfit = 0.0;
		ShortPositionProfit = 0.0;

		LastPrice = 0.0;
		PreSettlementPrice = 0.0;

	}
	std::string instId;//合约信息
	int LongPosition;//多头持仓数量
	int TodayLongPosition;//多头今仓数量
	int YdLongPosition;//多头昨仓数量
	double LongAvgEntryPrice;//多头开仓价格
	double LongAvgHoldingPrice;//多头持仓价格
	double LongCloseProfit;//多头平仓盈亏
	double LongPositionProfit;//多头持仓盈亏
	double LongOpenProfit;//多头开仓盈亏

	int ShortPosition;//空头持仓数量
	int TodayShortPosition;//空头今仓数量
	int YdShortPosition;//空头昨仓数量
	double ShortAvgEntryPrice;//空头开仓价格
	double ShortAvgHoldingPrice;//空头持仓价格
	double ShortCloseProfit;//空头平仓盈亏
	double ShortPositionProfit;//空头持仓盈亏
	double ShortOpenProfit;//空头开仓盈亏

	double LastPrice;//最新价
	double PreSettlementPrice;//昨结价

};


enum ESignal
{
	NoAction,
	BuyOpen,
	BuyToCover,
	BuyToCover_BuyOpen,
	BuyToCoverTD,
	BuyToCoverTD_BuyOpen,
	BuyToCoverYD,
	BuyToCoverYD_BuyOpen,
	SellOpen,
	Sell,
	Sell_SellOpen,
	SellTD,
	SellTD_SellOpen,
	SellYD,
	SellYD_SellOpen,
};
//tick数据
struct Tick
{
	std::string date;
	std::string time;
	///最后修改毫秒
	TThostFtdcMillisecType	UpdateMillisec;
	double bidPrice;
	int bidVolume;
	double askPrice;
	int askVolume;
	double vol;
	double openInterest;
};


//Kbar数据
struct Kbar
{
	std::string date;
	std::string time;
	//时间周期 多少分钟
	int Internval;
	double open;
	double high;
	double low;
	double close;
	double vol;
	double openInterest;
};
//读取k线
void ReadKbarSeries(std::string fileName, std::vector<Kbar>& kbar_vec);

//保存文件名
void Save_FileName(std::string path, std::vector<std::string>& fileName_vec);

void StringToCharPP(std::string & str,char *  pp);

int MKDir(std::string &strDirName);

int CreateFile(std::string strPath, char * cInst);
void CreateKBarFolder();
void CreateKbarDataFile(std::string strFileName);
void SaveTick(std::string strFolder, CThostFtdcDepthMarketDataField * pDepthMarketData);