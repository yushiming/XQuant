#pragma once
#include<string>
#include<vector>
#include"ThostFtdcUserApiDataType.h"
#include"ThostFtdcUserApiStruct.h"
//��λ��Ϣ�ṹ��
struct position_field
{
	position_field()
	{
		instId = "";

		LongPosition = 0;
		TodayLongPosition = 0;
		YdLongPosition = 0;
		LongAvgEntryPrice = 0.0;//��ֵ�ƽ�����ּ۸�
		LongAvgHoldingPrice = 0.0;
		LongCloseProfit = 0.0;
		LongOpenProfit = 0.0;
		LongPositionProfit = 0.0;

		ShortPosition = 0;
		TodayShortPosition = 0;
		YdShortPosition = 0;
		ShortAvgEntryPrice = 0.0;//�ղֵ�ƽ�����ּ۸�
		ShortAvgHoldingPrice = 0.0;
		ShortCloseProfit = 0.0;
		ShortOpenProfit = 0.0;
		ShortPositionProfit = 0.0;

		LastPrice = 0.0;
		PreSettlementPrice = 0.0;

	}
	std::string instId;//��Լ��Ϣ
	int LongPosition;//��ͷ�ֲ�����
	int TodayLongPosition;//��ͷ�������
	int YdLongPosition;//��ͷ�������
	double LongAvgEntryPrice;//��ͷ���ּ۸�
	double LongAvgHoldingPrice;//��ͷ�ֲּ۸�
	double LongCloseProfit;//��ͷƽ��ӯ��
	double LongPositionProfit;//��ͷ�ֲ�ӯ��
	double LongOpenProfit;//��ͷ����ӯ��

	int ShortPosition;//��ͷ�ֲ�����
	int TodayShortPosition;//��ͷ�������
	int YdShortPosition;//��ͷ�������
	double ShortAvgEntryPrice;//��ͷ���ּ۸�
	double ShortAvgHoldingPrice;//��ͷ�ֲּ۸�
	double ShortCloseProfit;//��ͷƽ��ӯ��
	double ShortPositionProfit;//��ͷ�ֲ�ӯ��
	double ShortOpenProfit;//��ͷ����ӯ��

	double LastPrice;//���¼�
	double PreSettlementPrice;//����

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
//tick����
struct Tick
{
	std::string date;
	std::string time;
	///����޸ĺ���
	TThostFtdcMillisecType	UpdateMillisec;
	double bidPrice;
	int bidVolume;
	double askPrice;
	int askVolume;
	double vol;
	double openInterest;
};


//Kbar����
struct Kbar
{
	std::string date;
	std::string time;
	//ʱ������ ���ٷ���
	int Internval;
	double open;
	double high;
	double low;
	double close;
	double vol;
	double openInterest;
};
//��ȡk��
void ReadKbarSeries(std::string fileName, std::vector<Kbar>& kbar_vec);

//�����ļ���
void Save_FileName(std::string path, std::vector<std::string>& fileName_vec);

void StringToCharPP(std::string & str,char *  pp);

int MKDir(std::string &strDirName);

int CreateFile(std::string strPath, char * cInst);
void CreateKBarFolder();
void CreateKbarDataFile(std::string strFileName);
void SaveTick(std::string strFolder, CThostFtdcDepthMarketDataField * pDepthMarketData);