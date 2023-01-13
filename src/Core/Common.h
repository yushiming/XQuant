
#pragma once

//  Copyright [2023] <xiaxianyue>
//=====================================================================================
//
//      Filename:  Common.h
//
//   Description:  ���������ࡢ������������
//
//       Version:  1.0
//       Created:  2023��1��2�� 12ʱ52��34��
//      Revision:  none
//      Compiler:  vs2019
//
//        Author:  yushiming
//  Organization:  xxy 
//
//=====================================================================================


#include <vector>
#include <string>
#include <map>

namespace XQuant {

	enum EPlatform : int {
		eNull             = 0,            // 
		eFutures          = 1,            // �ڻ�
		eStocks           = 2,            // ��Ʊ
		eForex            = 3,            // ���
		eDigitalCash      = 4,            // ���ֻ���
	};
	       
	enum ETrendTime : int {
		eTTNull         = 0,
		eTTTimeShare    = 1,
		eTTDay          = 2,
		eTTWeek         = 3,
		eTTMonth        = 4,
		eTTSeason       = 5,
		eTTYear         = 6,
		eTT1Minute      = 7,
		eTT5Minute      = 8,
		eTT15Minute     = 9,
		eTT30Minute     = 10,
		eTT60Minute     = 11,
	};

	extern std::map<EPlatform, std::string> PlatformName;

	extern std::map<ETrendTime, std::string> TrendTimeName;

	// contract �ڻ���Լ  IF2205
	struct FuturesAccountInfo
	{
		char appID[64];
		char authcode[64];
		char product[64];
		char brokerID[64];
		char userID[64];
		char password[64];
		char marketFront[64];
		char tradeFront[64];
	};

	struct StocksAccountInfo
	{
		char appID[64];
		char authcode[64];
		char product[64];
		char brokerID[64];
		char userID[64];
		char password[64];
		char marketFront[64];
		char tradeFront[64];
	};

	struct ForexAccountInfo
	{
		char appID[64];
		char authcode[64];
		char product[64];
		char brokerID[64];
		char userID[64];
		char password[64];
		char marketFront[64];
		char tradeFront[64];
	};

	struct DigitalCashAccountInfo
	{
		char appID[64];        
		char authcode[64];     
		char product[64];      
		char brokerID[64];     
		char userID[64];       
		char password[64];     
		char marketFront[64];  
		char tradeFront[64];   
	};



}