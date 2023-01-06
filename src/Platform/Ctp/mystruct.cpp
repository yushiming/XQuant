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

//stringת��char����
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
	//�ж��ļ����Ƿ����
//_access����0�����ļ�����
	if (0 != _access(strDirName.c_str(), 0))
	{
		//�����ļ��гɹ�������0
		nRet = _mkdir(strDirName.c_str());
	}
	return nRet;
}

int CreateFile(std::string strPath, char* cInst)
{
	char filePath[100] = { '\0' };
	//�����ļ���·��
	sprintf_s(filePath, sizeof(filePath), "%s%s_market_data.csv", strPath.c_str(), cInst);
	std::ofstream outFile;
	//�ж��ļ��Ƿ����
	//�ļ�����_access(filePath, 0)==0
	//�ļ�������
	if (0 != _access(filePath, 0))
	{
		outFile.open(filePath, ios::out);
		outFile << "��Լ����" << ","
			<< "����ʱ��" << ","
			<< "���¼�" << ","
			<< "�ɽ���" << ","
			<< "��һ��" << ","
			<< "��һ��" << ","
			<< "��һ��" << ","
			<< "��һ��" << ","
			<< "�ֲ���" << ","
			<< "������" << ","
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

	//����0����ɹ�
	int nRet = MKDir(strFolder);
	strFolder += "/Kbar";
		////����"E:/database/Kbar"
		nRet= MKDir(strFolder);
		if (!nRet)
			cerr << "�����ļ��гɹ�" << endl;
		else
			cerr << "�����ļ���ʧ��" << endl;
}

void CreateKbarDataFile(std::string strFilePath)
{
	char filePath[100] = { '\0' };
	strcpy(filePath, strFilePath.c_str());
	
	//au2106_1_Minute
	//�����ļ���·��
	sprintf_s(filePath, sizeof(filePath), "%s.csv", filePath);
	std::ofstream outFile;
	//�ж��ļ��Ƿ����
	//�ļ�����_access(filePath, 0)==0
	//�ļ�������
	if (0 != _access(filePath, 0))
	{
		outFile.open(filePath, ios::out);
		outFile << "��Լ����" << ","
			<< "ҵ������" << ","
			<< "������" << ","
			<< "��ʼʱ��" << ","
			<< "����ʱ��" << ","
			<< "���̼�" << ","
			<< "��߼�" << ","
			<< "��ͼ�" << ","
			<< "���̼�" << ","
			<< "����" << ","
			<< "�ɽ���" << ","
			<< "�ֲ���" << ","
			<< "�ֲ����仯"
			<< endl;
		outFile.close();
	}
}

void SaveTick(std::string strPath, CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	//���˿�����
	if (!pDepthMarketData)
		return;
	
	char filePath[100] = { '\0' };
	//�����ļ���·��
	sprintf_s(filePath, sizeof(filePath), "%s%s_market_data.csv", strPath.c_str(), pDepthMarketData->InstrumentID);
	std::ofstream outFile;
	outFile.open(filePath, ios::app);//�ļ�׷��д��
	
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
