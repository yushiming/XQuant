#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "mystruct.h"
#include"direct.h"
#include"io.h"
#include<fstream>
#include<iostream>
using namespace std;
void ReadKbarSeries(std::string fileName, std::vector<Kbar>& kbar_vec)
{
}

void Save_FileName(std::string path, std::vector<std::string>& fileName_vec)
{
}

//string转成char数组
void StringToCharPP(std::string & str, char  * pp)
{
	int len = str.length();
	pp = new char[len + 1];

	memcpy(pp, str.c_str(), len);
	pp[len] = '\0';
}

int MKDir(std::string& strDirName)
{
	int nRet = -1;
	//判断文件夹是否存在
//_access返回0代表文件存在
	if (0 != _access(strDirName.c_str(), 0))
	{
		//创建文件夹成功，返回0
		nRet = _mkdir(strDirName.c_str());
	}
	return nRet;
}

int CreateFile(std::string strPath, char* cInst)
{
	char filePath[100] = { '\0' };
	//生成文件的路径
	sprintf_s(filePath, sizeof(filePath), "%s%s_market_data.csv", strPath.c_str(), cInst);
	std::ofstream outFile;
	//判断文件是否存在
	//文件存在_access(filePath, 0)==0
	//文件不存在
	if (0 != _access(filePath, 0))
	{
		outFile.open(filePath, ios::out);
		outFile << "合约代码" << ","
			<< "更新时间" << ","
			<< "最新价" << ","
			<< "成交量" << ","
			<< "买一价" << ","
			<< "买一量" << ","
			<< "卖一价" << ","
			<< "卖一量" << ","
			<< "持仓量" << ","
			<< "换手率" << ","
			<< endl;
		outFile.close();
	}
	else
		return -1;
	return 0;
}

void CreateKBarFolder()
{
	string strFolder = "E:/database";

	//返回0代表成功
	int nRet = MKDir(strFolder);
	strFolder += "/Kbar";
		////生成"E:/database/Kbar"
		nRet= MKDir(strFolder);
		if (!nRet)
			cerr << "生成文件夹成功" << endl;
		else
			cerr << "生成文件夹失败" << endl;
}

void CreateKbarDataFile(std::string strFilePath)
{
	char filePath[100] = { '\0' };
	strcpy(filePath, strFilePath.c_str());
	
	//au2106_1_Minute
	//生成文件的路径
	sprintf_s(filePath, sizeof(filePath), "%s.csv", filePath);
	std::ofstream outFile;
	//判断文件是否存在
	//文件存在_access(filePath, 0)==0
	//文件不存在
	if (0 != _access(filePath, 0))
	{
		outFile.open(filePath, ios::out);
		outFile << "合约代码" << ","
			<< "业务日期" << ","
			<< "交易日" << ","
			<< "开始时间" << ","
			<< "结束时间" << ","
			<< "开盘价" << ","
			<< "最高价" << ","
			<< "最低价" << ","
			<< "收盘价" << ","
			<< "均价" << ","
			<< "成交量" << ","
			<< "持仓量" << ","
			<< "持仓量变化"
			<< endl;
		outFile.close();
	}
}

void SaveTick(std::string strPath, CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	//过滤空数据
	if (!pDepthMarketData)
		return;
	
	char filePath[100] = { '\0' };
	//生成文件的路径
	sprintf_s(filePath, sizeof(filePath), "%s%s_market_data.csv", strPath.c_str(), pDepthMarketData->InstrumentID);
	std::ofstream outFile;
	outFile.open(filePath, ios::app);//文件追加写入
	
	outFile << pDepthMarketData->InstrumentID << ","
		<< pDepthMarketData->UpdateTime << "." << pDepthMarketData->UpdateMillisec << ","
		<< pDepthMarketData->LastPrice << ","
		<< pDepthMarketData->Volume << ","
		<< pDepthMarketData->BidPrice1 << ","
		<< pDepthMarketData->BidVolume1 << ","
		<< pDepthMarketData->AskPrice1 << ","
		<< pDepthMarketData->AskVolume1 << ","
		<< pDepthMarketData->OpenInterest << ","
		<< pDepthMarketData->Turnover << std::endl;

	outFile.close();
}
