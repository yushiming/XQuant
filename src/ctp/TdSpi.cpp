#include "TdSpi.h"
#include<map>
#include<iostream>
#include<mutex>
#include"Strategy.h"
using namespace std;


extern std::map<std::string, std::string> accountConfig_map;//�����˻���Ϣ��map
//ȫ�ֵĻ�����
extern std::mutex m_mutex;
extern Strategy* g_strategy;//������ָ��

extern int g_nRequestID;

//ȫ�ֵĳֲֺ�Լ
extern std::vector<std::string> subscribe_inst_vec;

TdSpi::TdSpi(CThostFtdcTraderApi* tdapi, CThostFtdcMdApi* pUserApi_md, MdSpi* pUserSpi_md):
	m_pUserTDApi_trade(tdapi), m_pUserMDApi_trade(pUserApi_md), m_pUserMDSpi_trade(pUserSpi_md)

{
	
	m_AppId= accountConfig_map["appid"];
	m_AuthCode= accountConfig_map["authcode"];
	m_BrokerId = accountConfig_map["brokerId"];
	m_UserId = accountConfig_map["userId"];
	m_Passwd= accountConfig_map["passwd"];
	m_InstId= accountConfig_map["contract"];
	m_InstId1 = accountConfig_map["contract1"];
	m_InstId2 = accountConfig_map["contract2"];
	m_nNextRequestID=0;
	m_QryOrder_Once = true;

	m_QryTrade_Once = true;
	m_QryDetail_Once = true;
	m_QryTradingAccount_Once = true;
	m_QryPosition_Once = true;
	m_QryInstrument_Once = true;

	bUpdateMdPrice = false;

	//����ӯ��
	m_OpenProfit = 0.0;
	//ƽ��ӯ��
	m_CloseProfit = 0.0;
	//�ֲ�ӯ��
	m_PositionProfit = 0.0;
	
}

TdSpi::~TdSpi()
{
	Release();
}

void TdSpi::OnFrontConnected()
{
	cerr << "OnFrontConnected ��" << endl;
	static const char* version = m_pUserTDApi_trade->GetApiVersion();
	cerr << "��ǰ��CTP Api Version��" <<version<< endl;
	ReqAuthenticate();
}



int TdSpi::ReqAuthenticate()
{
	//virtual int ReqAuthenticate(CThostFtdcReqAuthenticateField * pReqAuthenticateField, int nRequestID) = 0;
	CThostFtdcReqAuthenticateField req;
	//��ʼ��
	memset(&req, 0, sizeof(req));


	strcpy(req.AppID, m_AppId.c_str());
	//strcpy(req.AppID, "eiruoejladkfj");
	strcpy(req.AuthCode, m_AuthCode.c_str());
	//strcpy(req.AuthCode, "eiruoejladkfj");
	strcpy(req.BrokerID, m_BrokerId.c_str());
	//strcpy(req.BrokerID,"0000");
	//strcpy(req.UserID, m_UserId.c_str());
	strcpy(req.UserID, "");
	cerr << "������֤���˻���Ϣ��" << endl << " appid: " << req.AppID << " authcode: " << req.AuthCode
		<< " brokerid: " << req.BrokerID << " userId: " << req.UserID << endl;
	
	return m_pUserTDApi_trade->ReqAuthenticate(&req,GetNextRequestID());
}
void TdSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo)
	{
		if (pRspInfo->ErrorID == 0)
		{
			cerr << "��͸������֤�ɹ���" << "ErrMsg:" << pRspInfo->ErrorMsg << endl;
			ReqUserLogin();
		}

		else
		{
			cerr << "��͸������֤ʧ�ܣ�" << " errorid:" << pRspInfo->ErrorID <<
				"ErrMsg:" << pRspInfo->ErrorMsg << endl;

			//<error id = "AUTH_FAILED" value = "63" prompt = "CTP:�ͻ�����֤ʧ��" / >
		}
	}

}
int TdSpi::ReqUserLogin()
{
	cerr << "====ReqUserLogin====,�û���¼��..." <<endl;
	//����һ��CThostFtdcReqUserLoginField
	CThostFtdcReqUserLoginField reqUserLogin;
	//��ʼ��Ϊ0
	memset(&reqUserLogin, 0, sizeof(reqUserLogin));
	//����brokerid,userid,passwd
	strcpy(reqUserLogin.BrokerID, m_BrokerId.c_str());
	//strcpy(reqUserLogin.BrokerID, "0000");
	//errorid:3�����Ϸ��ĵ�¼
	//strcpy(reqUserLogin.UserID, "00000000");
	strcpy(reqUserLogin.UserID, m_UserId.c_str());
	strcpy(reqUserLogin.Password, m_Passwd.c_str());

	//strcpy(reqUserLogin.Password, "00000000");
	
	//��¼
	return m_pUserTDApi_trade->ReqUserLogin(&reqUserLogin, GetNextRequestID());

}

void TdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "��¼����ص�OnRspUserLogin" << endl;
	if (!IsErrorRspInfo(pRspInfo) && pRspUserLogin)
	{
		m_nFrontID = pRspUserLogin->FrontID;
		m_nSessionID = pRspUserLogin->SessionID;
		int nextOrderRef = atoi(pRspUserLogin->MaxOrderRef);

		sprintf_s(orderRef, sizeof(orderRef), "%012d", ++nextOrderRef);

		cout << "ǰ�ñ��:" << pRspUserLogin->FrontID << endl
			<< "�Ự���" << pRspUserLogin->SessionID << endl
			<< "��󱨵�����:" << pRspUserLogin->MaxOrderRef << endl
			<< "������ʱ�䣺" << pRspUserLogin->SHFETime << endl
			<< "������ʱ�䣺" << pRspUserLogin->DCETime << endl
			<< "֣����ʱ�䣺" << pRspUserLogin->CZCETime << endl
			<< "�н���ʱ�䣺" << pRspUserLogin->FFEXTime << endl
			<< "��Դ��ʱ�䣺" << pRspUserLogin->INETime << endl
			<< "�����գ�" << m_pUserTDApi_trade->GetTradingDay() << endl;
		strcpy(m_cTradingDay, m_pUserTDApi_trade->GetTradingDay());//���ý�������
		
		cout << "--------------------------------------------" << endl << endl;
	}

	ReqSettlementInfoConfirm();

}


void TdSpi::OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{


}
void TdSpi::OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
}
void TdSpi::ReqQryOrder()
{
	CThostFtdcQryOrderField  QryOrderField;//����
	memset(&QryOrderField, 0, sizeof(CThostFtdcQryOrderField));//��ʼ��Ϊ0
	//brokerid����
	//strcpy(QryOrderField.BrokerID, "0000");
	strcpy(QryOrderField.BrokerID, m_BrokerId.c_str());
	//InvestorID����
	//strcpy(QryOrderField.InvestorID, "666666");
	strcpy(QryOrderField.InvestorID, m_UserId.c_str());
	//����api��ReqQryOrder
	m_pUserTDApi_trade->ReqQryOrder(&QryOrderField, GetNextRequestID());

}

void TdSpi::OnRspQryOrder(CThostFtdcOrderField* pOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "�����ѯ������Ӧ��OnRspQryOrder" <<",pOrder  "<<pOrder<< endl;
	if (!IsErrorRspInfo(pRspInfo) && pOrder)
	{
		cerr << "�����ѯ������Ӧ��ǰ�ñ��FrontID��" << pOrder->FrontID << ",�Ự���:" << pOrder->SessionID
			<< ",��������:  " << pOrder->OrderRef << endl;
			//���к�Լ
		if (m_QryOrder_Once == true)
		{
			CThostFtdcOrderField* order = new CThostFtdcOrderField();
			memcpy(order, pOrder, sizeof(CThostFtdcOrderField));
			orderList.insert(make_pair(pOrder->BrokerOrderSeq, order));
			
			
			
			//bIsLast�Ƿ������һ�ʻر�
			if (bIsLast)
			{
				m_QryOrder_Once = false;
				cerr << "���к�Լ�ı�������" << orderList.size() << endl;
				cerr << "---------------��ӡ������ʼ---------------" << endl;
				for (auto iter = orderList.begin(); iter != orderList.end(); iter++)
				{
				/*	cerr << "���͹�˾���룺" << (iter->second)->BrokerID << "  " << "Ͷ���ߴ��룺" << (iter->second)->InvestorID << "  "
						<< "�û����룺" << (iter->second)->UserID << "  " << "��Լ���룺" << (iter->second)->InstrumentID << "  "
						<< "��������" << (iter->second)->Direction << "  " << "��Ͽ�ƽ��־��" << (iter->second)->CombOffsetFlag << "  "
						<< "�۸�" << (iter->second)->LimitPrice << "  " << "������" << (iter->second)->VolumeTotalOriginal << "  "
						<< "�������ã�" << (iter->second)->OrderRef << "  " << "�ͻ����룺" << (iter->second)->ClientID << "  "
						<< "����״̬��" << (iter->second)->OrderStatus << "  " << "ί��ʱ�䣺" << (iter->second)->InsertTime << "  "
						<< "������ţ�" << (iter->second)->OrderStatus << "  " << "�����գ�" << (iter->second)->TradingDay << "  "
						<< "�������ڣ�" << (iter->second)->InsertDate << endl;*/

				}
				cerr << "---------------��ӡ�������---------------" << endl;
				cerr << "��ѯ�������������״β�ѯ�ɽ�" << endl;
				

			}
		}

	}
	else//��ѯ����
	{
		m_QryOrder_Once = false;
		cerr << "��ѯ����������û�гɽ������״β�ѯ�ɽ�" << endl;
		
	}
	if (bIsLast)
	{
		//�߳�����3�룬��ctp��̨�г������Ӧʱ�䣬Ȼ���ٽ��в�ѯ����
		std::chrono::milliseconds sleepDuration(3 * 1000);
		std::this_thread::sleep_for(sleepDuration);
		ReqQryTrade();
	}
}


void TdSpi::ReqQryTrade()
{
	CThostFtdcQryTradeField tdField;//����
	memset(&tdField, 0, sizeof(tdField));//��ʼ��

	strcpy(tdField.BrokerID, m_BrokerId.c_str());
	//strcpy(tdField.BrokerID,"0000");
	strcpy(tdField.InvestorID, m_UserId.c_str());
	//strcpy(tdField.InvestorID, "888888");
	//���ý���api��ReqQryTrade
	m_pUserTDApi_trade->ReqQryTrade(&tdField, GetNextRequestID());
}
void TdSpi::OnRspQryTrade(CThostFtdcTradeField* pTrade, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{

	cerr << "�����ѯ�ɽ��ر���Ӧ��OnRspQryTrade" << " pTrade " << pTrade << endl;

	if (!IsErrorRspInfo(pRspInfo) && pTrade)
	{
		//���к�Լ
		if (m_QryTrade_Once == true)
		{
			CThostFtdcTradeField* trade = new CThostFtdcTradeField();//����trade
			memcpy(trade, pTrade, sizeof(CThostFtdcTradeField));//pTrade���Ƹ�trade
			tradeList.push_back(trade);

			if (bIsLast)
			{
				m_QryTrade_Once = false;
				cerr << "���к�Լ�ĳɽ�����" << tradeList.size() << endl;
				cerr << "---------------��ӡ�ɽ���ʼ---------------" << endl;
				for (vector<CThostFtdcTradeField*>::iterator iter = tradeList.begin(); iter != tradeList.end(); iter++)
				{
					cerr << "���͹�˾���룺" << (*iter)->BrokerID << endl << "Ͷ���ߴ��룺" << (*iter)->InvestorID << endl
						<< "�û����룺" << (*iter)->UserID << endl << "�ɽ���ţ�" << (*iter)->TradeID << endl
						<< "��Լ���룺" << (*iter)->InstrumentID << endl << "��������" << (*iter)->Direction << endl
						<< "��Ͽ�ƽ��־��" << (*iter)->OffsetFlag << endl << "Ͷ���ױ���־��" << (*iter)->HedgeFlag << endl
						<< "�۸�" << (*iter)->Price << endl << "������" << (*iter)->Volume << endl
						<< "�������ã�" << (*iter)->OrderRef << endl << "���ر�����ţ�" << (*iter)->OrderLocalID << endl
						<< "�ɽ�ʱ�䣺" << (*iter)->TradeTime << endl << "ҵ��Ԫ��" << (*iter)->BusinessUnit << endl
						<< "��ţ�" << (*iter)->SequenceNo << endl << "���͹�˾�µ���ţ�" << (*iter)->BrokerOrderSeq << endl
						<< "�����գ�" << (*iter)->TradingDay << endl;

				}
				cerr << "---------------��ӡ�ɽ����---------------" << endl;
				cerr << "��ѯ�������������״β�ѯ�ֲ���ϸ" << endl;
				
			}
			

		}

	}
	else//��ѯ����
	{
		m_QryOrder_Once = false;
		cerr << "��ѯ����������û�гɽ������״β�ѯ�ɽ�" << endl;
		
	}
	if (bIsLast)
	{
		//�߳�����3�룬��ctp��̨�г������Ӧʱ�䣬Ȼ���ٽ��в�ѯ����
		std::chrono::milliseconds sleepDuration(3 * 1000);
		std::this_thread::sleep_for(sleepDuration);
		ReqQryInvestorPositionDetail();
	}

	
}
void TdSpi::ReqQryInvestorPositionDetail()
{
	CThostFtdcQryInvestorPositionDetailField pdField;//����
	memset(&pdField, 0, sizeof(pdField));//��ʼ��Ϊ0
	strcpy(pdField.BrokerID, m_BrokerId.c_str());
	//strcpy(pdField.BrokerID, "0000");
	//strcpy(pdField.InstrumentID, m_InstId.c_str());


	strcpy(pdField.InvestorID, m_UserId.c_str());

	//strcpy(pdField.InvestorID, "0000");
	//���ý���api��ReqQryInvestorPositionDetail
	m_pUserTDApi_trade->ReqQryInvestorPositionDetail(&pdField,GetNextRequestID());
}

void TdSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField* pField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "�����ѯͶ���ֲ߳���ϸ�ر���Ӧ��OnRspQryInvestorPositionDetail" << " pInvestorPositionDetail " << pField << endl;
	if (!IsErrorRspInfo(pRspInfo) && pField)
	{
		//���к�Լ
		if (m_QryDetail_Once == true)
		{
			//�������к�Լ��ֻ����δƽ�ֵģ��������Ѿ�ƽ�ֵ�
			//����������ǰ�ĳֲּ�¼���浽δƽ������tradeList_NotClosed_Long��tradeList_NotClosed_Short
			//ʹ�ýṹ��CThostFtdcTradeField����Ϊ����ʱ���ֶΣ���CThostFtdcInvestorPositionDetailFieldû��ʱ���ֶ�
			CThostFtdcTradeField* trade = new CThostFtdcTradeField();//����CThostFtdcTradeField *
			
			strcpy(trade->InvestorID, pField->InvestorID);///Ͷ���ߴ���
			strcpy(trade->InstrumentID, pField->InstrumentID);///��Լ����
			strcpy(trade->ExchangeID, pField->ExchangeID);///����������
			trade->Direction = pField->Direction;//��������
			trade->Price = pField->OpenPrice;//�۸�
			trade->Volume = pField->Volume;//����
			strcpy(trade->TradeDate, pField->OpenDate);//�ɽ�����
			//�����ӣ�TradingDay����ֶΣ�Ҫ����
			strcpy(trade->TradingDay, pField->TradingDay);//�����գ��н����ǽ��죬��ʹ����ֲ���ϸ�����
			strcpy(trade->TradeID, pField->TradeID);//*********�ɽ����********
			if (pField->Volume > 0)//ɸѡδƽ�ֺ�Լ
			{
				if (trade->Direction == '0')//���뷽��
				{
					if (positionDetailList_NotClosed_Long.size() > 0)
					{
						std::list< CThostFtdcTradeField*>::const_iterator it = positionDetailList_NotClosed_Long.cbegin();
						while( it != positionDetailList_NotClosed_Long.cend())
						{
							//const_reverse_iterator
							if (CompareRule(trade, *it))
							{
								positionDetailList_NotClosed_Long.insert(it, trade);
								break;
							}
							it++;
						}
						if(it== positionDetailList_NotClosed_Long.cend())
							positionDetailList_NotClosed_Long.push_back(trade);

					}
					else
					positionDetailList_NotClosed_Long.push_front(trade);
				}
					
				else if (trade->Direction == '1')//��������
				{
					if (positionDetailList_NotClosed_Short.size() > 0)
					{
						std::list< CThostFtdcTradeField*>::const_iterator it = positionDetailList_NotClosed_Short.cbegin();
						while (it != positionDetailList_NotClosed_Short.cend())
						{
							//const_reverse_iterator
							if (CompareRule(trade, *it))
							{
								positionDetailList_NotClosed_Short.insert(it, trade);
								break;
							}
							it++;
						}
						if (it == positionDetailList_NotClosed_Short.cend())
							positionDetailList_NotClosed_Short.push_back(trade);

					}
					else
						positionDetailList_NotClosed_Short.push_front(trade);
				}
					
			}
			//�ռ��ֲֺ�Լ�Ĵ���
			bool find_instId = false;
			for (unsigned int i = 0; i < subscribe_inst_vec.size(); i++)
			{
				if (strcmp(subscribe_inst_vec[i].c_str(), trade->InstrumentID) == 0)//��Լ�Ѵ��ڣ��Ѷ���
				{
					find_instId = true;
					break;
				}
			}
			if (!find_instId)//��Լδ���Ĺ�
			{
				cerr << "---------------------------------------�óֲֺ�Լδ���Ĺ������붩���б�" << endl;
				subscribe_inst_vec.push_back(trade->InstrumentID);
			}

		}
		//������к�Լ�ĳֲ���ϸ��Ҫ����߽�����һ���Ĳ�ѯReqQryTradingAccount()
		if (bIsLast)
		{
			m_QryDetail_Once = false;
			//�ֲֵĺ�Լ
			string inst_holding;
			//
			for (unsigned int i = 0; i < subscribe_inst_vec.size(); i++)
				inst_holding = inst_holding + subscribe_inst_vec[i] + ",";
			//"IF2102,IF2103,"

			inst_holding = inst_holding.substr(0, inst_holding.length() - 1);//ȥ�����Ķ��ţ���λ��0��ʼ��ѡȡlength-1���ַ�
			//"IF2102,IF2103"

			cerr << "��������ǰ�ĳֲ��б�:" << inst_holding << ",inst_holding.length()=" << inst_holding.length()
				<< ",subscribe_inst_vec.size()=" << subscribe_inst_vec.size() << endl;

			if (inst_holding.length() > 0)
				m_pUserMDSpi_trade->setInstIdList_Position_MD(inst_holding);//���ó�������ǰ�����֣�����Ҫ��������ĺ�Լ

			//size�������������������
			cerr << "�˻����к�Լδƽ�ֵ��������µ�������һ�ʿ��Զ�Ӧ���֣�,�൥:" << positionDetailList_NotClosed_Long.size()
				<< "�յ���" << positionDetailList_NotClosed_Short.size() << endl;

			//positionDetailList_NotClosed_Long.sort(CThostFtdcTradeField::compareRule);
			cerr << "-----------------------------------------δƽ�ֶ൥��ϸ��ӡstart" << endl;
			for (auto iter = positionDetailList_NotClosed_Long.begin(); iter != positionDetailList_NotClosed_Long.end(); iter++)
			{
				cerr << "BrokerID:" << (*iter)->BrokerID << "  " << "InvestorID:" << (*iter)->InvestorID << "  "
					<< "InstrumentID:" << (*iter)->InstrumentID << "  " << "Direction:" << (*iter)->Direction << "  "
					<< "OpenPrice:" << (*iter)->Price << "  " << "Volume:" << (*iter)->Volume << "  "
					<< "TradeDate:" << (*iter)->TradeDate << "  " << "TradeID:" << (*iter)->TradeID << endl;
			}

			cerr << "-----------------------------------------δƽ�ֿյ���ϸ��ӡstart" << endl;
			for (auto iter = positionDetailList_NotClosed_Short.begin(); iter != positionDetailList_NotClosed_Short.end(); iter++)
			{
				cerr << "BrokerID:" << (*iter)->BrokerID << "  " << "InvestorID:" << (*iter)->InvestorID << "  "
					<< "InstrumentID:" << (*iter)->InstrumentID << "  " << "Direction:" << (*iter)->Direction << "  "
					<< "OpenPrice:" << (*iter)->Price << "  " << "Volume:" << (*iter)->Volume << "  "
					<< "TradeDate:" << (*iter)->TradeDate << "  " << "TradeID:" << (*iter)->TradeID << endl;
			}
			cerr << "---------------��ӡ�ֲ���ϸ���---------------" << endl;
			cerr << "��ѯ�ֲ���ϸ���������״β�ѯ�˻��ʽ���Ϣ" << endl;
		}
		
	}
	else
	{
		if (m_QryDetail_Once == true)
		{
			m_QryDetail_Once = false;
			cerr << "��ѯ�ֲ���ϸ������û�гֲ���ϸ�����״β�ѯ�˻��ʽ�" << endl;
		}
	}
	if (bIsLast)
	{
		//�߳�����3�룬��ctp��̨�г������Ӧʱ�䣬Ȼ���ٽ��в�ѯ����
		std::chrono::milliseconds sleepDuration(3 * 1000);
		std::this_thread::sleep_for(sleepDuration);
		ReqQryTradingAccount();
	}
	
}

void TdSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;//����req�Ľṹ�����
	memset(&req, 0, sizeof(req));//��ʼ��
	//�����brokerID
	//strcpy(req.BrokerID, "8888");


	strcpy(req.BrokerID, m_BrokerId.c_str());


	strcpy(req.InvestorID, m_UserId.c_str());
	//strcpy(req.InvestorID, "666666");
	//���ý���api��ReqQryTradingAccount
	int iResult = m_pUserTDApi_trade->ReqQryTradingAccount(&req, GetNextRequestID());
	cerr << "--->>> �����ѯ�ʽ��˻�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void TdSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField* pTradingAccount, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "�����ѯͶ�����ʽ��˻��ر���Ӧ��OnRspQryTradingAccount" << " pTradingAccount " << pTradingAccount << endl;
	if (!IsErrorRspInfo(pRspInfo) && pTradingAccount)
	{

		cerr << "Ͷ���߱�ţ�" << pTradingAccount->AccountID
			<< "��̬Ȩ�棺�ڳ�Ȩ��" << pTradingAccount->PreBalance
			<< "��̬Ȩ�棺�ڻ�����׼����" << pTradingAccount->Balance
			<< "�����ʽ�" << pTradingAccount->Available
			<< "��ȡ�ʽ�" << pTradingAccount->WithdrawQuota
			<< "��ǰ��֤���ܶ" << pTradingAccount->CurrMargin
			<< "ƽ��ӯ����" << pTradingAccount->CloseProfit
			<< "�ֲ�ӯ����" << pTradingAccount->PositionProfit
			<< "�����ѣ�" << pTradingAccount->Commission
			<< "���ᱣ֤��" << pTradingAccount->FrozenCash
			<< endl;
		//���к�Լ
		if (m_QryTradingAccount_Once == true)
		{
			m_QryTradingAccount_Once = false;
		}

		cerr << "---------------��ӡ�ʽ��˻���ϸ���---------------" << endl;
		cerr << "��ѯ�ʽ��˻����������״β�ѯͶ���ֲ߳���Ϣ" << endl;
	}
	else
	{
		if (m_QryTradingAccount_Once == true)
		{
			m_QryTradingAccount_Once = false;
			cerr << "��ѯ�ʽ��˻��������״β�ѯͶ���ֲ߳�" << endl;
		}
	}
	//�߳�����3�룬��ctp��̨�г������Ӧʱ�䣬Ȼ���ٽ��в�ѯ����
	std::chrono::milliseconds sleepDuration(3 * 1000);
	std::this_thread::sleep_for(sleepDuration);
	ReqQryInvestorPosition_All();
}
//���ֻ�����Լ��µ��ĳ����вŻ��лص����ڵ������Ŀͻ����п�����
void TdSpi::OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspOrderInsert" << endl;
	printf("failed to insert order,ins: %s, vol: %d, price:%f, orderref:%s,requestid:%d,ErrorID: %d, errmsg: %s\n"
		, pInputOrder->InstrumentID, pInputOrder->VolumeTotalOriginal, pInputOrder->LimitPrice, pInputOrder->OrderRef, pInputOrder->RequestID
		, pRspInfo->ErrorID, pRspInfo->ErrorMsg);

	g_strategy->OnRspOrderInsert(pInputOrder);
}


void TdSpi::ReqQryInvestorPosition_All()
{
	CThostFtdcQryInvestorPositionField req;//����req
	memset(&req, 0, sizeof(req));//��ʼ��Ϊ0

	//strcpy(req.BrokerID, "8888");
	strcpy(req.BrokerID, m_BrokerId.c_str());
	strcpy(req.InvestorID, m_UserId.c_str());
	//strcpy(req.InvestorID, "0000");
	//��ԼΪ�գ�������ѯ���к�Լ�ĳֲ֣������reqΪ����һ����
	//strcpy(req.InstrumentID, m_InstId.c_str());
	//���ý���api��ReqQryInvestorPosition
	int iResult = m_pUserTDApi_trade->ReqQryInvestorPosition(&req, GetNextRequestID());//reqΪ�գ������ѯ���к�Լ�ĳֲ�
	cerr << "--->>> �����ѯͶ���ֲ߳�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}
void TdSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField* pInvestorPosition,
	CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	//cerr << "�����ѯ�ֲ���Ӧ��OnRspQryInvestorPosition " << ",pInvestorPosition  " << pInvestorPosition << endl;
	if (!IsErrorRspInfo(pRspInfo) && pInvestorPosition)
	{

		//�˻������к�Լ
		if (m_QryPosition_Once == true)
		{
			cerr << "�����ѯ�ֲ���Ӧ��OnRspQryInvestorPosition " << " pInvestorPosition:  "
				<< pInvestorPosition << endl;//������Ѿ�ƽ��û�гֲֵļ�¼
			cerr << "��Ӧ  | ��Լ " << pInvestorPosition->InstrumentID << endl
				<< " �ֲֶ�շ��� " << pInvestorPosition->PosiDirection << endl//2��3��
			   // << " ӳ���ķ��� " << MapDirection(pInvestorPosition->PosiDirection-2,false) << endl
				<< " �ֲܳ� " << pInvestorPosition->Position << endl
				<< " ���ճֲ� " << pInvestorPosition->TodayPosition << endl
				<< " ���ճֲ� " << pInvestorPosition->YdPosition << endl
				<< " ��֤�� " << pInvestorPosition->UseMargin << endl
				<< " �ֲֳɱ� " << pInvestorPosition->PositionCost << endl
				<< " ������ " << pInvestorPosition->OpenVolume << endl
				<< " ƽ���� " << pInvestorPosition->CloseVolume << endl
				<< " �ֲ����� " << pInvestorPosition->TradingDay << endl
				<< " ƽ��ӯ��������ᣩ " << pInvestorPosition->CloseProfitByDate << endl
				<< " �ֲ�ӯ�� " << pInvestorPosition->PositionProfit << endl
				<< " ���ն���ƽ��ӯ��������ᣩ " << pInvestorPosition->CloseProfitByDate << endl//��������ʾ�������ֵ
				<< " ��ʶԳ�ƽ��ӯ��������ƽ��Լ�� " << pInvestorPosition->CloseProfitByTrade << endl//�ڽ����бȽ�������
				<< endl;


			//�����Լ��Ӧ�ֲ���ϸ��Ϣ�Ľṹ��map
			bool  find_trade_message_map = false;
			for (map<string, position_field*>::iterator iter = m_position_field_map.begin(); iter != m_position_field_map.end(); iter++)
			{
				if (strcmp((iter->first).c_str(), pInvestorPosition->InstrumentID) == 0)//��Լ�Ѵ���
				{
					find_trade_message_map = true;
					break;
				}
			}
			if (!find_trade_message_map)//��Լ������
			{
				cerr << "-----------------------û�������Լ����Ҫ���콻����Ϣ�ṹ��" << endl;
				position_field* p_trade_message = new position_field();
				p_trade_message->instId = pInvestorPosition->InstrumentID;
				//����ֲֺ�Լ��string
				m_Inst_Position += pInvestorPosition->InstrumentID ;
				m_Inst_Position += ",";
				m_position_field_map.insert(pair<string, position_field*>(pInvestorPosition->InstrumentID, p_trade_message));
			}
			if (pInvestorPosition->PosiDirection == '2')//�൥
			{
				//��ֺͽ��һ�η���
				//��ȡ�ú�Լ�ĳֲ���ϸ��Ϣ�ṹ�� second; m_map[��]
				position_field* p_tdm = m_position_field_map[pInvestorPosition->InstrumentID];
				p_tdm->LongPosition = p_tdm->LongPosition + pInvestorPosition->Position;
				p_tdm->TodayLongPosition = p_tdm->TodayLongPosition + pInvestorPosition->TodayPosition;
				p_tdm->YdLongPosition = p_tdm->LongPosition - p_tdm->TodayLongPosition;
				p_tdm->LongCloseProfit = p_tdm->LongCloseProfit + pInvestorPosition->CloseProfit;
				p_tdm->LongPositionProfit = p_tdm->LongPositionProfit + pInvestorPosition->PositionProfit;
			}
			else if (pInvestorPosition->PosiDirection == '3')//�յ�
			{
				//��ֺͽ��һ�η���

				position_field* p_tdm = m_position_field_map[pInvestorPosition->InstrumentID];
				p_tdm->ShortPosition = p_tdm->ShortPosition + pInvestorPosition->Position;
				p_tdm->TodayShortPosition = p_tdm->TodayShortPosition + pInvestorPosition->TodayPosition;
				p_tdm->YdShortPosition = p_tdm->ShortPosition - p_tdm->TodayShortPosition;
				p_tdm->ShortCloseProfit = p_tdm->ShortCloseProfit + pInvestorPosition->CloseProfit;
				p_tdm->ShortPositionProfit = p_tdm->ShortPositionProfit + pInvestorPosition->PositionProfit;
			}

			if (bIsLast)
			{
				m_QryPosition_Once = false;
				m_Inst_Position = m_Inst_Position.substr(0, m_Inst_Position.length() - 1);
				
				//m_pUserMDSpi_trade->setInstIdList_Position_MD(m_Inst_Postion);
				
				for (map<string, position_field*>::iterator iter = m_position_field_map.begin(); iter != m_position_field_map.end(); iter++)
				{
					cerr << "��Լ���룺" << iter->second->instId << endl
						<< "�൥�ֲ�����" << iter->second->LongPosition << endl
						<< "�յ��ֲ�����" << iter->second->ShortPosition << endl
						<< "�൥���ճֲ֣�" << iter->second->TodayLongPosition << endl
						<< "�൥���ճֲ֣�" << iter->second->YdLongPosition << endl
						<< "�յ����ճֲ֣�" << iter->second->TodayShortPosition << endl
						<< "�յ����ճֲ֣�" << iter->second->YdShortPosition << endl
						<< "�൥����ӯ����" << iter->second->LongPositionProfit << endl
						<< "�൥ƽ��ӯ����" << iter->second->LongCloseProfit << endl
						<< "�յ�����ӯ����" << iter->second->ShortPositionProfit << endl
						<< "�յ�ƽ��ӯ����" << iter->second->ShortCloseProfit << endl;

					//�˻�ƽ��ӯ��
					m_CloseProfit = m_CloseProfit + iter->second->LongCloseProfit + iter->second->ShortCloseProfit;
					//�˻�����ӯ��
					m_OpenProfit = m_OpenProfit + iter->second->LongOpenProfit + iter->second->ShortOpenProfit;

					//�˻��ĳֲ�ӯ��
					m_PositionProfit = m_PositionProfit + iter->second->LongPositionProfit + iter->second->ShortPositionProfit;
				}

				cerr << "�˻�����ӯ�� " << m_OpenProfit << endl;
				cerr << "�˻�ƽ��ӯ�� " << m_CloseProfit << endl;
			}//bisLast


		}
		cerr << "---------------��ѯͶ���ֲ߳����---------------" << endl;
		cerr << "��ѯ�ֲ��������״β�ѯ���к�Լ����" << endl;
	}
	else
	{
		if (m_QryPosition_Once == true)
			m_QryPosition_Once = false;
		cerr << "��ѯͶ���ֲֳ߳�����û�гֲ֣��״β�ѯ���к�Լ" << endl;
	}
	if (bIsLast)
	{
		//�߳�����3�룬��ctp��̨�г������Ӧʱ�䣬Ȼ���ٽ��в�ѯ����
		std::chrono::milliseconds sleepDuration(10 * 1000);
		std::this_thread::sleep_for(sleepDuration);
		ReqQryInstrumetAll();
	}
	
}

/// <summary>
/// ��ѯ�����ڻ���Լ
/// </summary>
void TdSpi::ReqQryInvestorPosition(char * pInstrument)
{
	CThostFtdcQryInvestorPositionField req;//����req
	memset(&req, 0, sizeof(req));//��ʼ��Ϊ0

	
	strcpy(req.BrokerID, m_BrokerId.c_str());
	strcpy(req.InvestorID, m_UserId.c_str());
	
	//��Լ��д����ĺ�Լ����
	strcpy(req.InstrumentID, pInstrument);
	//���ý���api��ReqQryInvestorPosition
	int iResult = m_pUserTDApi_trade->ReqQryInvestorPosition(&req, GetNextRequestID());//reqΪ�գ������ѯ���к�Լ�ĳֲ�
	cerr << "--->>> �����ѯͶ���ֲ߳�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void TdSpi::ReqQryInstrumetAll()
{
	CThostFtdcQryInstrumentField req;//����req
	memset(&req, 0, sizeof(req));//��ʼ��Ϊ0


	//���ý���api��ReqQryInstrument
	int iResult = m_pUserTDApi_trade->ReqQryInstrument(&req, GetNextRequestID());//req�ṹ��Ϊ0����ѯ���к�Լ
	cerr << "--->>> �����ѯ��Լ: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void TdSpi::ReqQryInstrumet(char * pInstrument)
{
	CThostFtdcQryInstrumentField req;//����req
	memset(&req, 0, sizeof(req));//��ʼ��Ϊ0
	strcpy(req.InstrumentID, pInstrument);//��Լ��д����Ĵ��룬��ʾ��ѯ�ú�Լ����Ϣ
	//���ý���api��ReqQryInstrument
	int iResult = m_pUserTDApi_trade->ReqQryInstrument(&req, GetNextRequestID());//
	cerr << "--->>> �����ѯ��Լ: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}



void TdSpi::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << "�����ѯ��Լ��Ӧ��OnRspQryInstrument" << ",pInstrument   " << pInstrument->InstrumentID << endl;
	if (!IsErrorRspInfo(pRspInfo) && pInstrument)
	{

		//�˻������к�Լ
		if (m_QryInstrument_Once == true)
		{
			m_Instrument_All = m_Instrument_All + pInstrument->InstrumentID + ",";

			if (pInstrument->ProductClass == '1')
				//(pInstrument->ProductClass == '1' || pInstrument->ProductClass == '3')
			{
				//�������к�Լ��Ϣ��map
				CThostFtdcInstrumentField* pInstField = new CThostFtdcInstrumentField();
				memcpy(pInstField, pInstrument, sizeof(CThostFtdcInstrumentField));
				m_all_instruments_field_map.insert(pair<string, CThostFtdcInstrumentField*>(pInstrument->InstrumentID, pInstField));
			}
			//���Խ��׵ĺ�Լ
			if (strcmp(m_InstId.c_str(), pInstrument->InstrumentID) == 0)
			{
				cerr << "��Ӧ | ��Լ��" << pInstrument->InstrumentID
					<< "��Լ���ƣ�" << pInstrument->InstrumentName
					<< " ��Լ�ڽ��������룺" << pInstrument->ExchangeInstID
					<< " ��Ʒ���룺" << pInstrument->ProductID
					<< " ��Ʒ���ͣ�" << pInstrument->ProductClass
					<< " ��ͷ��֤���ʣ�" << pInstrument->LongMarginRatio
					<< " ��ͷ��֤���ʣ�" << pInstrument->ShortMarginRatio
					<< " ��Լ����������" << pInstrument->VolumeMultiple
					<< " ��С�䶯��λ��" << pInstrument->PriceTick
					<< " ���������룺" << pInstrument->ExchangeID
					<< " ������ݣ�" << pInstrument->DeliveryYear
					<< " �����£�" << pInstrument->DeliveryMonth
					<< " �����գ�" << pInstrument->CreateDate
					<< " �����գ�" << pInstrument->ExpireDate
					<< " �����գ�" << pInstrument->OpenDate
					<< " ��ʼ�����գ�" << pInstrument->StartDelivDate
					<< " ���������գ�" << pInstrument->EndDelivDate
					<< " ��Լ��������״̬��" << pInstrument->InstLifePhase
					<< " ��ǰ�Ƿ��ף�" << pInstrument->IsTrading << endl;
			}

			if (bIsLast)
			{
				m_QryInstrument_Once = false;
				m_Instrument_All = m_Instrument_All.substr(0, m_Instrument_All.length() - 1);
				cerr << "m_Instrument_All�Ĵ�С��" << m_Instrument_All.size() << endl;
				cerr << "map�Ĵ�С����Լ��������" << m_all_instruments_field_map.size() << endl;

				//���ֲֺ�Լ��Ϣ���õ�mdspi
				//m_pUserMDSpi_trade->setInstIdList_Position_MD(m_Inst_Postion);


				//����Լ��Ϣ�ṹ���map���Ƶ�������
				g_strategy->set_instPostion_map_stgy(m_all_instruments_field_map);
				cerr << "--------------------------�����Լ��Ϣmap������-----------------------" << endl;
				//ShowInstMessage();
				//����ȫ�г���Լ����TD���У���Ҫ����ȫ�г���Լ����ʱ������
				m_pUserMDSpi_trade->set_InstIdList_All(m_Instrument_All);
				cerr << "TD��ʼ����ɣ�����MD" << endl;
				m_pUserMDApi_trade->Init();
			}
		}
	}
	else
	{
		m_QryInstrument_Once = false;
		cerr << "��ѯ��Լʧ��" << endl;
	}
	

}


void TdSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;//����
	memset(&req, 0, sizeof(req));//��ʼ��
	strcpy(req.BrokerID, m_BrokerId.c_str());
	//strcpy(req.BrokerID, "0000");
	strcpy(req.InvestorID, m_UserId.c_str());
	//strcpy(req.InvestorID, "000000");
	
	int iResult = m_pUserTDApi_trade->ReqSettlementInfoConfirm(&req, GetNextRequestID());
	cerr << "--->>> Ͷ���߽�����ȷ��: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}


void TdSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	
	if (bIsLast && !IsErrorRspInfo(pRspInfo) && pSettlementInfoConfirm)
	{
		cerr << "��Ӧ | ���㵥..." << pSettlementInfoConfirm->InvestorID
			<< "..." << pSettlementInfoConfirm->ConfirmDate << "," <<
			pSettlementInfoConfirm->ConfirmTime << "...ȷ��" << endl << endl;

		cerr << "���㵥ȷ���������״β�ѯ����" << endl;
		//�߳�����3�룬��ctp��̨�г������Ӧʱ�䣬Ȼ���ٽ��в�ѯ����
		std::chrono::milliseconds sleepDuration(3 * 1000);
		std::this_thread::sleep_for(sleepDuration);
		//Sleep(1000);
		ReqQryOrder();
	}
}

bool TdSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
	
}




void TdSpi::OnRspQrySettlementInfo(CThostFtdcSettlementInfoField* pSettlementInfo, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
}


void TdSpi::OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField* pUserPasswordUpdate, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
}

void TdSpi::OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
}








void TdSpi::OnRtnOrder(CThostFtdcOrderField* pOrder)
{
	char* pszStatus = new char[13];
	switch (pOrder->OrderStatus) {
	case THOST_FTDC_OST_AllTraded:
		strcpy(pszStatus, "ȫ���ɽ�");
		break;
	case THOST_FTDC_OST_PartTradedQueueing:
		strcpy(pszStatus, "���ֳɽ�");
		break;
	case THOST_FTDC_OST_NoTradeQueueing:
		strcpy(pszStatus, "δ�ɽ�");
		break;
	case THOST_FTDC_OST_Canceled:
		strcpy(pszStatus, "�ѳ���");
		break;
	case THOST_FTDC_OST_Unknown:
		strcpy(pszStatus, "δ֪");
		break;
	case THOST_FTDC_OST_NotTouched:
		strcpy(pszStatus, "δ����");
		break;
	case THOST_FTDC_OST_Touched:

		strcpy(pszStatus, "�Ѵ���");
		break;
	default:
		strcpy(pszStatus, "");
		break;
	}
	
	cerr << "BrokerOrderSeq:" << pOrder->BrokerOrderSeq << "  ,OrderRef;" << pOrder->OrderRef << " ,����״̬  "
		<< pszStatus << ",��ƽ��־  " << pOrder->CombOffsetFlag[0] <<
		",�����ύ״̬  " << pOrder->OrderSubmitStatus <<
		",����״̬��Ϣ  " << pOrder->StatusMsg << endl << endl;
	
	
	//һ������ȫ���ı���
	//1�����ȱ��沢�����������
	UpdateOrder(pOrder, orderList);
	

	//2����ӡ���еı���

	//ShowOrderList(orderList);

	
	//������������ı���
	//�ж��Ƿ񱾳��򷢳��ı�����

	if (pOrder->FrontID != m_nFrontID || pOrder->SessionID != m_nSessionID) {

		CThostFtdcOrderField* pOld = GetOrder(pOrder->BrokerOrderSeq);
		//����Ƕ��������������򷢳��ĵ��ӣ�����m_Orders��¼��,pOld��Ϊ��
		if (pOld == NULL) {
			return;
		}
		
	}

	
	//1�����ȱ��沢�����������
	UpdateOrder(pOrder, m_Orders);

	//2����m_Orders��ӡ����
	//ShowOrderList(m_Orders);

	g_strategy->OnRtnOrder(pOrder);
	
	
	//*********************************�������Դ���*************************
	//�Ѿ��ɽ����Ѿ������ĵ��ӣ��ٳ�һ�Σ������Ƿ�ᱨ��
	/*if (pOrder->OrderStatus == '0' || pOrder->OrderStatus == '5')
	{
		CThostFtdcOrderField* pOld = GetOrder(pOrder->BrokerOrderSeq);
		CancelOrder(pOrder);
	}*/
	//*********************************�������Դ���*************************



	///*printf("order returned,ins: %s, vol: %d, price:%f, orderref:%s,requestid:%d,traded vol: %d,ExchID:%s, OrderSysID:%s,status: %s,statusmsg:%s\n"
	//	, pOrder->InstrumentID, pOrder->VolumeTotalOriginal, pOrder->LimitPrice, pOrder->OrderRef, pOrder->RequestID
	//	, pOrder->VolumeTraded, pOrder->ExchangeID, pOrder->OrderSysID, pszStatus, pOrder->StatusMsg);*/
	////���沢���±�����״̬
	//UpdateOrder(pOrder);


	/*cerr <<"BrokerOrderSeq:"<< pOrder->BrokerOrderSeq<< "  ,OrderRef;" <<pOrder->OrderRef<<" ,����״̬  "
		<< pszStatus <<",��ƽ��־  "<<pOrder->CombOffsetFlag[0]<< 
		",�����ύ״̬  "<< pOrder->OrderSubmitStatus <<
		",����״̬��Ϣ  " << pOrder->StatusMsg << endl <<endl;*/

	//if (pOrder->OrderStatus == '3'|| pOrder->OrderStatus == '1')
	//{
	//	CThostFtdcOrderField* pOld = GetOrder(pOrder->BrokerOrderSeq);
	//	if (pOld && pOld->OrderStatus != '6')
	//	{
	//		cerr << "onRtnOrder ׼��������:" << endl;
	//		CancelOrder(pOrder);
	//	}
	//	
	//	
	//}
	
}

//void TdSpi::OnRspOrderAction(CThostFtdcInputOrderActionField* pInputOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
//{
//	cerr << "OnRspOrderAction:" << endl;
//}

void TdSpi::OnErrRtnOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo)
{
	
	cerr << "--------------OnErrRtnOrderInsert----------------:" << endl;
	printf("failed to insert order,ins: %s, vol: %d, price:%f, orderref:%s,requestid:%d,ErrorID: %d, errmsg: %s\n"
		, pInputOrder->InstrumentID, pInputOrder->VolumeTotalOriginal, pInputOrder->LimitPrice, pInputOrder->OrderRef, pInputOrder->RequestID
		, pRspInfo->ErrorID, pRspInfo->ErrorMsg);
}

void TdSpi::OnRtnTrade(CThostFtdcTradeField* pTrade)
{
	
	//һ�����º�Լ��Ϣ�ṹ���ӳ��
	UpdatePositionFieldMap(pTrade->InstrumentID);
	
	
	//��������ɽ���tradeList���˻����еĳɽ��б�
	//1����Ҫ�ж��Ƿ��Ƕ���������
	bool bFind = false;
	//1���жϸñʽ����Ƿ����
	bFind=FindTrade(pTrade);

	//2�����û�м�¼�������ɽ����ݣ���дһ������ɽ��ĺ���
	if (!bFind)
	{
		InsertTrade(pTrade);
		ShowTradeList();
	}
	else//3���ɽ���¼�Ѿ����ڣ��򷵻أ�������
		return;
		
	//�������³ֲ���ϸ
	//1������ʽ��׼�¼ȥ�������ǵĳֲ���ϸ
	UpdatePositionDetail(pTrade);
		
	//2����ӡ�ֲ���ϸ
	//ShowPositionDetail();
	
	//�ġ������˻��ֲ���Ϣ
	//1���жϺ�Լ�Ƿ���
	bool findInst = FindInstInSubVec(pTrade->InstrumentID);

	//2�����ĺ�Լ
	if (!findInst)
	{
		string strInst = pTrade->InstrumentID;
		subscribe_inst_vec.push_back(strInst);
		SubscribeMarketData(strInst);
	}
	//3�����³ֲ���Ϣ
	UpdatePosition();

	//4����ӡ�ֲ���Ϣ
	if(bUpdateMdPrice=BeUpDatedMDPrice())
	//ShowPosition();

	//***************************************************���±�����ĳɽ���Ϣ*************************************
	
	//1���ж��Ƿ�����ش���
	//���������������Ժ󣬳ɽ����ٴ�ˢ��һ��
	if (FindTrade(pTrade,m_Trades))
		//����ɽ���¼���ڣ��򷵻أ�������
		return;

	//2���ж��Ƿ񱾳��򷢳��ı�����
	CThostFtdcOrderField* pOrder = GetOrder(pTrade->BrokerOrderSeq);
	if (pOrder != NULL) {
		//	//ֻ�������򷢳��ı�����
		InsertTrade(pTrade,m_Trades);
		ShowTradeList(m_Trades);

		g_strategy->OnRtnTrade(pTrade);
	}
	//	
	//	printf("trade returned,ins: %s, trade vol: %d, trade price:%f, ExchID:%s, OrderSysID:%s\n"
	//		, pTrade->InstrumentID, pTrade->Volume, pTrade->Price, pTrade->ExchangeID, pTrade->OrderSysID);

	//	//g_strategy->OnRtnTrade(pTrade);

	//}
}

void TdSpi::OnRspOrderAction(CThostFtdcInputOrderActionField* pInputOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << endl << endl;
	cerr << "------------------OnRspOrderAction-------------------" << endl;
	printf("failed to cancel order,ins: %s, vol: %d, price:%f, orderref:%s,orderActionref:%d,requestid:%d,ErrorID: %d, errmsg: %s\n"
		, pInputOrderAction->InstrumentID, pInputOrderAction->VolumeChange, pInputOrderAction->LimitPrice, pInputOrderAction->OrderRef, pInputOrderAction->OrderActionRef, pInputOrderAction->RequestID
		, pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	cerr << endl << endl;
}
void TdSpi::OnErrRtnOrderAction(CThostFtdcOrderActionField* pOrderAction, CThostFtdcRspInfoField* pRspInfo)
{
	cerr << endl << endl;
	cerr << "-----------------OnErrRtnOrderAction---------------------" << endl;
	printf("failed to cancel order,ins: %s, vol: %d, price:%f, orderref:%s,orderActionref:%d,requestid:%d,ErrorID: %d, errmsg: %s\n"
		, pOrderAction->InstrumentID, pOrderAction->VolumeChange, pOrderAction->LimitPrice, pOrderAction->OrderRef, pOrderAction->OrderActionRef, pOrderAction->RequestID
		, pRspInfo->ErrorID, pRspInfo->ErrorMsg);
	cerr << endl << endl;
}

void TdSpi::CancelOrder(CThostFtdcOrderField* pOrder)
{
	CThostFtdcInputOrderActionField oa;//����һ�������Ľṹ�����
	memset(&oa, 0, sizeof(CThostFtdcInputOrderActionField));//��ʼ�����ֶ�����
	
	oa.ActionFlag = THOST_FTDC_AF_Delete;//����
	//�����������ֶΣ���ȷ�����ǵı���
	oa.FrontID = pOrder->FrontID;//ǰ�ñ��
	oa.SessionID = pOrder->SessionID;//�Ự
	strcpy(oa.OrderRef, pOrder->OrderRef);//��������

	if (pOrder->ExchangeID[0] != '\0') {
		strcpy(oa.ExchangeID, pOrder->ExchangeID);
	}
	if (pOrder->OrderSysID[0] != '\0') {
		strcpy(oa.OrderSysID, pOrder->OrderSysID);
	}

	strcpy(oa.BrokerID, pOrder->BrokerID);
	strcpy(oa.UserID, pOrder->UserID);
	strcpy(oa.InstrumentID, pOrder->InstrumentID);
	strcpy(oa.InvestorID, pOrder->InvestorID);

	//oa.RequestID = pOrder->RequestID;
	oa.RequestID = GetNextRequestID();
	//���ý���api�ĳ�������
	int nRetCode = m_pUserTDApi_trade->ReqOrderAction(&oa, oa.RequestID);

	char* pszStatus = new char[13];
	switch (pOrder->OrderStatus) {
	case THOST_FTDC_OST_AllTraded:
		strcpy(pszStatus, "ȫ���ɽ�");
		break;
	case THOST_FTDC_OST_PartTradedQueueing:
		strcpy(pszStatus, "���ֳɽ�");
		break;
	case THOST_FTDC_OST_NoTradeQueueing:
		strcpy(pszStatus, "δ�ɽ�");
		break;
	case THOST_FTDC_OST_Canceled:
		strcpy(pszStatus, "�ѳ���");
		break;
	case THOST_FTDC_OST_Unknown:
		strcpy(pszStatus, "δ֪");
		break;
	case THOST_FTDC_OST_NotTouched:
		strcpy(pszStatus, "δ����");
		break;
	case THOST_FTDC_OST_Touched:

		strcpy(pszStatus, "�Ѵ���");
		break;
	default:
		strcpy(pszStatus, "");
		break;
	}
	/*printf("����ing,ins: %s, vol: %d, price:%f, orderref:%s,requestid:%d,traded vol: %d,ExchID:%s, OrderSysID:%s,status: %s,statusmsg:%s\n"
		, pOrder->InstrumentID, pOrder->VolumeTotalOriginal, pOrder->LimitPrice, pOrder->OrderRef, pOrder->RequestID
		, pOrder->VolumeTraded, pOrder->ExchangeID, pOrder->OrderSysID, pszStatus, pOrder->StatusMsg);*/
	//cerr << "TdSpi::CancelOrder ����ing" << pszStatus << endl;
	if (nRetCode != 0) {
		printf("cancel order failed.\n");
	}
	else
	{
		if (pOrder->OrderStatus != '0' && pOrder->OrderStatus != '5')
		{
			pOrder->OrderStatus = '6';//��6����ʾ����;��
			UpdateOrder(pOrder);
			cerr << "TdSpi::CancelOrder ����ing" << pszStatus << endl;
			cerr << "TdSpi::CancelOrder ״̬��Ϊ pOrder->OrderStatus :" << pOrder->OrderStatus << endl;
		}
	}
	
	
}

void TdSpi::ShowPosition()
{
	cerr << endl << endl<<endl;
	cerr << "��ӡ�˻��ֲ�------------------------------------------" << endl;
	for (auto iter = m_position_field_map.begin(); iter != m_position_field_map.end(); iter++)
	{
		if (iter->second->LongPosition + iter->second->ShortPosition > 0)
		{
			cerr << "��Լ���룺" << iter->second->instId << " ";
			if (iter->second->LongPosition > 0)
				cerr << "�൥�ֲ�����" << iter->second->LongPosition << " "
				<< "�൥���ճֲ֣�" << iter->second->TodayLongPosition << " "
				<< "�൥���ճֲ֣�" << iter->second->YdLongPosition << " "
				<< "�൥���־��ۣ�" << iter->second->LongAvgEntryPrice << " "
				<< "�൥�ֲ־��ۣ�" << iter->second->LongAvgHoldingPrice << " "
				<< "�൥�ֲ�ӯ����" << iter->second->LongPositionProfit << " "
				<< "�൥����ӯ����" << iter->second->LongOpenProfit << " "
				<< "�൥ƽ��ӯ����" << iter->second->LongCloseProfit << " ";
			if (iter->second->ShortPosition > 0)
				cerr << "�յ��ֲ�����" << iter->second->ShortPosition << " "
				<< "�յ����ճֲ֣�" << iter->second->TodayShortPosition << " "
				<< "�յ����ճֲ֣�" << iter->second->YdShortPosition << " "
				<< "�յ����־��ۣ�" << iter->second->ShortAvgEntryPrice << " "
				<< "�յ��ֲ־��ۣ�" << iter->second->ShortAvgHoldingPrice << " "
				<< "�յ��ֲ�ӯ����" << iter->second->ShortPositionProfit << " "
				<< "�յ�����ӯ����" << iter->second->ShortOpenProfit << " "
				<< "�յ�ƽ��ӯ����" << iter->second->ShortCloseProfit << " ";

				cerr<< "���¼ۣ�" << iter->second->LastPrice << " "
				<< "���ۣ�" << iter->second->PreSettlementPrice << " "
				<< endl;
		}
			

		
	}
	cerr << endl << endl << endl;
}

void TdSpi::ClosePosition()
{

}

void TdSpi::SetAllowOpen(bool isOk)
{

}
CThostFtdcOrderField* TdSpi::GetOrder(CThostFtdcOrderField* pOrder)
{
	CThostFtdcOrderField* pOrder1 = NULL;//����һ�������ṹ��ָ��
	std::lock_guard<std::mutex> m_lock(m_mutex);//����
	auto it = m_Orders.begin();
	for (; it != m_Orders.end(); it++)
	{
		if (pOrder->FrontID == it->second->FrontID && pOrder->SessionID == it->second->SessionID && strcmp(pOrder->OrderRef, it->second->OrderRef) == 0)
			break;
	}
	//�ҵ���
	if (it != m_Orders.end()) {
		pOrder1 = it->second;
	}

	return pOrder1;
}
CThostFtdcOrderField* TdSpi::GetOrder(int nBrokerOrderSeq)
{
	CThostFtdcOrderField* pOrder = NULL;//����һ�������ṹ��ָ��
	std::lock_guard<std::mutex> m_lock(m_mutex);//����
	map<int, CThostFtdcOrderField*>::iterator it = m_Orders.find(nBrokerOrderSeq);//
	//�ҵ���
	if (it != m_Orders.end()) {
		pOrder = it->second;
	}

	return pOrder;
}

bool TdSpi::UpdateOrder(CThostFtdcOrderField* pOrder)
{
	//���͹�˾���µ����к�,����0��ʾ�Ѿ����ܱ���
	if (pOrder->BrokerOrderSeq > 0) 
	{
		std::lock_guard<std::mutex> m_lock(m_mutex);//��������֤���ӳ�����ݵİ�ȫ
		//�������������Ƿ����������
		map<int, CThostFtdcOrderField*>::iterator it = m_Orders.find(pOrder->BrokerOrderSeq);
		//������ڣ�������Ҫ��������״̬
		if (it != m_Orders.end()) 
			//if (it != orderMap.end() && strcmp((it->second)->BrokerID, pOrder->BrokerID) == 0)
		{
			CThostFtdcOrderField* pOld = it->second;//�ѽṹ���ָ�븳ֵ��pOld
			//ԭ�����Ѿ��رգ�
			char cOldStatus = pOld->OrderStatus;
			switch (cOldStatus) 
			{
			case THOST_FTDC_OST_AllTraded://ȫ���ɽ�
			case THOST_FTDC_OST_Canceled://�ѳ���
			case THOST_FTDC_OST_Touched://�Ѿ�����
				return false;
			case '6'://canceling//�Լ�����ģ��������Ѿ������˳������󣬻���;��
				if (pOrder->OrderStatus != '0' && pOrder->OrderStatus != '5')
					return false;
			}
			memcpy(pOld, pOrder, sizeof(CThostFtdcOrderField));//���±�����״̬
			cerr << "TdSpi::UpdateOrder pOrder->OrderStatus :" << (it->second)->OrderStatus << endl;
			
			
		}
		//��������ڣ�������Ҫ�������������Ϣ
		else 
		{
			CThostFtdcOrderField* pNew = new CThostFtdcOrderField();
			memcpy(pNew, pOrder, sizeof(CThostFtdcOrderField));
			m_Orders.insert(pair<int, CThostFtdcOrderField*>(pNew->BrokerOrderSeq, pNew));
		}
		return true;
	}
	//����Ļ��Ͳ��ü��뵽ӳ��
	return false;
}

bool TdSpi::UpdateOrder(CThostFtdcOrderField* pOrder, std::map<int,CThostFtdcOrderField*>& orderMap)
{
	//���͹�˾���µ����к�,����0��ʾ�Ѿ����ܱ���
	if (pOrder->BrokerOrderSeq > 0)
	{
		std::lock_guard<std::mutex> m_lock(m_mutex);//��������֤���ӳ�����ݵİ�ȫ
		//�������������Ƿ����������
		map<int, CThostFtdcOrderField*>::iterator it = orderMap.find(pOrder->BrokerOrderSeq);
		//������ڣ�������Ҫ��������״̬
		if (it != orderMap.end())
			//if (it != orderMap.end() && strcmp((it->second)->BrokerID, pOrder->BrokerID) == 0)
		{
			CThostFtdcOrderField* pOld = it->second;//�ѽṹ���ָ�븳ֵ��pOld
			//ԭ�����Ѿ��رգ�
			char cOldStatus = pOld->OrderStatus;
			switch (cOldStatus)
			{
			case THOST_FTDC_OST_AllTraded://ȫ���ɽ�0
			case THOST_FTDC_OST_Canceled://�ѳ���5
			case THOST_FTDC_OST_Touched://�Ѿ�����c
				return false;
			case '6'://canceling//�Լ�����ģ��������Ѿ������˳������󣬻���;��
				if (pOrder->OrderStatus != '0' && pOrder->OrderStatus != '5')
					return false;
			}
			memcpy(pOld, pOrder, sizeof(CThostFtdcOrderField));//���±�����״̬
			cerr << "TdSpi::UpdateOrder pOrder->OrderStatus :" << (it->second)->OrderStatus << endl;


		}
		//��������ڣ�������Ҫ�������������Ϣ
		else
		{
			CThostFtdcOrderField* pNew = new CThostFtdcOrderField();
			memcpy(pNew, pOrder, sizeof(CThostFtdcOrderField));
			orderMap.insert(pair<int, CThostFtdcOrderField*>(pNew->BrokerOrderSeq, pNew));
		}
		return true;
	}
	//����Ļ��Ͳ��ü��뵽ӳ��
	return false;
}


/// <summary>
/// �жϸóֲ���ϸ�Ƿ�ɽ�ʱ�����
/// </summary>
/// <param name="_tf">��Ҫ����ĳֲ���ϸ</param>
/// <param name="_curr">�Ѿ���ӳ���е�ĳ�ʳֲ���ϸ</param>
/// <returns>�Ƿ����</returns>
bool TdSpi::CompareRule(const CThostFtdcTradeField* _tf, const CThostFtdcTradeField* _curr)
{
			
	//�����ж�����
		if (std::strcmp(_tf->TradeDate, _curr->TradeDate) < 0)
			return true;

		else if (std::strcmp(_tf->TradeDate, _curr->TradeDate) == 0)
		{
			//����жϽ���������
			if (std::strcmp(_tf->ExchangeID, _curr->ExchangeID) < 0)
				return true;
			else if (std::strcmp(_tf->ExchangeID, _curr->ExchangeID) == 0)
			{
				//�����ж� TradeID
				if (std::strcmp(_tf->TradeID, _curr->TradeID) < 0)
					return true;

			}

		}
		return false;
}


int TdSpi::GetNextRequestID()
{
	//��m_nNextRequestID���ϻ�����
	/*m_mutex.lock();
	int nNextID = m_nNextRequestID++;
	m_mutex.unlock();*/
	//1ԭ���ڹ��캯������ʹ��m_mutex.lock();
	//��������ʱ��ʹ�ý���m_mutex.unlock();
std:lock_guard<mutex> m_lock(m_mutex);
	
	int nNextID = g_nRequestID++;

	return g_nRequestID;
}
char * TdSpi::GetNextOrderRef()
{
	//����
	
std:lock_guard<mutex> m_lock(m_mutex);

	int nNextOrderRef =atoi(orderRef);

	sprintf_s(orderRef, sizeof(orderRef), "%012d", ++nNextOrderRef);

	return orderRef;
}
//
//bool TdSpi::compareRule(CThostFtdcTradeField* _left, CThostFtdcTradeField* _right)
//{
//	//�����ж�����
//	if (strcmp(_left->TradeDate, _right->TradeDate) < 0)
//		return true;
//
//	else if (strcmp(_left->TradeDate, _right->TradeDate) == 0)
//	{
//		//����жϽ���������
//		if (strcmp(_left->ExchangeID, _right->ExchangeID) < 0)
//			return true;
//		else if (strcmp(_left->ExchangeID, _right->ExchangeID) == 0)
//		{
//			//�����ж� TradeID
//			if (strcmp(_left->TradeID, _right->TradeID) < 0)
//				return true;
//
//		}
//
//	}
//	return false;
//}

void TdSpi::ShowInstMessage()
{
	//std::map<std::string, CThostFtdcInstrumentField*> m_inst_field_map;

	for (std::map<std::string, CThostFtdcInstrumentField*>::iterator iter = m_all_instruments_field_map.begin(); iter != m_all_instruments_field_map.end(); iter++)
	{
		CThostFtdcInstrumentField* pInstrument = iter->second;
		
		cerr << "��Ӧ | ��Լ��" << pInstrument->InstrumentID
			<< "��Լ���ƣ�" << pInstrument->InstrumentName
			<< " ��Լ�ڽ��������룺" << pInstrument->ExchangeInstID
			<< " ��Ʒ���룺" << pInstrument->ProductID
			<< " ��Ʒ���ͣ�" << pInstrument->ProductClass
			<< " ��ͷ��֤���ʣ�" << pInstrument->LongMarginRatio
			<< " ��ͷ��֤���ʣ�" << pInstrument->ShortMarginRatio
			<< " ��Լ����������" << pInstrument->VolumeMultiple
			<< " ��С�䶯��λ��" << pInstrument->PriceTick
			<< " ���������룺" << pInstrument->ExchangeID
			<< " ������ݣ�" << pInstrument->DeliveryYear
			<< " �����£�" << pInstrument->DeliveryMonth
			<< " �����գ�" << pInstrument->CreateDate
			<< " �����գ�" << pInstrument->ExpireDate
			<< " �����գ�" << pInstrument->OpenDate
			<< " ��ʼ�����գ�" << pInstrument->StartDelivDate
			<< " ���������գ�" << pInstrument->EndDelivDate
			<< " ��Լ��������״̬��" << pInstrument->InstLifePhase
			<< " ��ǰ�Ƿ��ף�" << pInstrument->IsTrading << endl;
	}
}

bool TdSpi::FindTrade(CThostFtdcTradeField* pTrade)
{
	//ExchangeID //����������
	//	TradeID   //�ɽ����
	//	Direction  //��������
	std::lock_guard<std::mutex> m_lock(m_mutex);//��������֤���set���ݵİ�ȫ
	for (auto it = tradeList.begin(); it != tradeList.end(); it++)
	{
		//�ж��Ƿ��Ѿ�����
		if (strcmp((*it)->ExchangeID, pTrade->ExchangeID) ==0&& 
			strcmp((*it)->TradeID, pTrade->TradeID) == 0&& (*it)->Direction== pTrade->Direction)
			return true;
	}

	return false;
}

bool TdSpi::FindTrade(CThostFtdcTradeField* pTrade, std::vector<CThostFtdcTradeField*>& tdList)
{
	//ExchangeID //����������
	//	TradeID   //�ɽ����
	//	Direction  //��������
	std::lock_guard<std::mutex> m_lock(m_mutex);//��������֤���set���ݵİ�ȫ
	for (auto it = tdList.begin(); it != tdList.end(); it++)
	{
		//�ж��Ƿ��Ѿ�����
		if (strcmp((*it)->ExchangeID, pTrade->ExchangeID) == 0 &&
			strcmp((*it)->TradeID, pTrade->TradeID) == 0 && (*it)->Direction == pTrade->Direction)
			return true;
	}

	return false;
}

void TdSpi::InsertTrade(CThostFtdcTradeField* pTrade)
{
	std::lock_guard<std::mutex> m_lock(m_mutex);//��������֤���set���ݵİ�ȫ
	CThostFtdcTradeField* trade = new CThostFtdcTradeField();//����trade������ѿռ䣬�ǵ���������������Ҫdelete
	memcpy(trade, pTrade, sizeof(CThostFtdcTradeField));//pTrade���Ƹ�trade
	tradeList.push_back(trade);//����¼��
}

void TdSpi::InsertTrade(CThostFtdcTradeField* pTrade, std::vector<CThostFtdcTradeField*>& tdList)
{
	std::lock_guard<std::mutex> m_lock(m_mutex);//��������֤���set���ݵİ�ȫ
	CThostFtdcTradeField* trade = new CThostFtdcTradeField();//����trade������ѿռ䣬�ǵ���������������Ҫdelete
	memcpy(trade, pTrade, sizeof(CThostFtdcTradeField));//pTrade���Ƹ�trade
	tdList.push_back(trade);
}

void TdSpi::ShowTradeList()
{
	std::lock_guard<std::mutex> m_lock(m_mutex);//��������֤���set���ݵİ�ȫ
	cerr << endl << endl;
	cerr << "---------------��ӡ�ɽ���ϸ-------------" << endl;
	for (auto iter = tradeList.begin(); iter != tradeList.end(); iter++)
	{
		cerr << endl << "Ͷ���ߴ��룺" << (*iter)->InvestorID << "  "
			<< "�û����룺" << (*iter)->UserID << "  " << "�ɽ���ţ�" << (*iter)->TradeID << "  "
			<< "��Լ���룺" << (*iter)->InstrumentID << "  " << "��������" << (*iter)->Direction << "  "
			<< "��ƽ��" << (*iter)->OffsetFlag << "  " << "Ͷ��/�ױ�" << (*iter)->HedgeFlag << "  "
			<< "�۸�" << (*iter)->Price << "  " << "������" << (*iter)->Volume << "  "
			<< "�������ã�" << (*iter)->OrderRef << "  " << "���ر�����ţ�" << (*iter)->OrderLocalID << "  "
			<< "�ɽ�ʱ�䣺" << (*iter)->TradeTime << "  " << "ҵ��Ԫ��" << (*iter)->BusinessUnit << "  "
			<< "��ţ�" << (*iter)->SequenceNo << "  " << "���͹�˾�µ���ţ�" << (*iter)->BrokerOrderSeq << "  "
			<< "�����գ�" << (*iter)->TradingDay << endl;
	}

	cerr << endl << endl;
}

void TdSpi::ShowTradeList(std::vector<CThostFtdcTradeField*>& tdList)
{
	std::lock_guard<std::mutex> m_lock(m_mutex);//��������֤���set���ݵİ�ȫ
	cerr << endl << endl;
	cerr << "---------------��ӡ�ɽ���ϸ-------------" << endl;
	for (auto iter = tdList.begin(); iter != tdList.end(); iter++)
	{
		cerr << endl << "Ͷ���ߴ��룺" << (*iter)->InvestorID << "  "
			<< "�û����룺" << (*iter)->UserID << "  " << "�ɽ���ţ�" << (*iter)->TradeID << "  "
			<< "��Լ���룺" << (*iter)->InstrumentID << "  " << "��������" << (*iter)->Direction << "  "
			<< "��ƽ��" << (*iter)->OffsetFlag << "  " << "Ͷ��/�ױ�" << (*iter)->HedgeFlag << "  "
			<< "�۸�" << (*iter)->Price << "  " << "������" << (*iter)->Volume << "  "
			<< "�������ã�" << (*iter)->OrderRef << "  " << "���ر�����ţ�" << (*iter)->OrderLocalID << "  "
			<< "�ɽ�ʱ�䣺" << (*iter)->TradeTime << "  " << "ҵ��Ԫ��" << (*iter)->BusinessUnit << "  "
			<< "��ţ�" << (*iter)->SequenceNo << "  " << "���͹�˾�µ���ţ�" << (*iter)->BrokerOrderSeq << "  "
			<< "�����գ�" << (*iter)->TradingDay << endl;
	}

	cerr << endl << endl;
}

void TdSpi::ShowOrderList(std::map<int, CThostFtdcOrderField*>& orderList)
{
	if (orderList.size() > 0)
	{
		cerr << endl<<endl<<endl;
		cerr << "�ܵı���������" << orderList.size() << endl;
		cerr << "---------------��ӡ������ʼ---------------" << endl;
		for (auto iter = orderList.begin(); iter != orderList.end(); iter++)
		{
			cerr << "���͹�˾���룺" << (iter->second)->BrokerID << "  " << "Ͷ���ߴ��룺" << (iter->second)->InvestorID << "  "
				<< "�û����룺" << (iter->second)->UserID << "  " << "��Լ���룺" << (iter->second)->InstrumentID << "  "
				<< "��������" << (iter->second)->Direction << "  " << "��Ͽ�ƽ��־��" << (iter->second)->CombOffsetFlag << "  "
				<< "�۸�" << (iter->second)->LimitPrice << "  " << "������" << (iter->second)->VolumeTotalOriginal << "  "
				<< "�������ã�" << (iter->second)->OrderRef << "  " << "�ͻ����룺" << (iter->second)->ClientID << "  "
				<< "����״̬��" << (iter->second)->OrderStatus << "  " << "ί��ʱ�䣺" << (iter->second)->InsertTime << "  "
				<< "������ţ�" << (iter->second)->OrderStatus << "  " << "�����գ�" << (iter->second)->TradingDay << "  "
				<< "�������ڣ�" << (iter->second)->InsertDate << endl;

		}
		cerr << "---------------��ӡ�������---------------" << endl;
		cerr << endl << endl << endl;
	}
}

//���³ֲ���ϸ
bool TdSpi::UpdatePositionDetail(CThostFtdcTradeField* pTrade)
{
	//����Ϊ�գ������·���false
	if (!pTrade)
		return false;

	std::lock_guard<std::mutex> m_lock(m_mutex);//��������֤������ݵİ�ȫ
	
	
												
	//1������ǿ��֣������tradeΪ�µĳֲ���ϸ
	if (pTrade->TradeID && pTrade->OffsetFlag == THOST_FTDC_OF_Open)
	{
		CThostFtdcTradeField* trade = new CThostFtdcTradeField();//����CThostFtdcTradeField *���������ڴ�ռ�
		memcpy(trade, pTrade, sizeof(CThostFtdcTradeField));
		//�ڳֲ���ϸ�ṹ���У�������trade->TradeDate������ñʳɽ������Ľ�����,��ͬ��openDate�����ֵĽ����գ�
		strcpy(trade->TradeDate, m_cTradingDay);//�Ϻ��ڻ����ף�tradingday��Ҳ����ȷ��
		strcpy(trade->TradingDay, m_cTradingDay);//��Ӧģ�⻷����trade->TradingDay��Ϣ��Щ�������ǿյ�trade->TradingDay==""
		if (pTrade->Direction == '0')//���뷽��
			positionDetailList_NotClosed_Long.push_back(trade);
		else if (pTrade->Direction == '1')//��������
			positionDetailList_NotClosed_Short.push_back(trade);
	}
	else//�����ƽ����ɾ����Ӧ�ĳֲ���ϸ
	{
		int vol = pTrade->Volume;//�ɽ�����
		
		std::list<CThostFtdcTradeField*>* pos = nullptr;

		if (pTrade->Direction == '0')//���뷽������ƽ�֣��յ��ĳֲ���ϸ
		{
			pos = &positionDetailList_NotClosed_Short;
		}
		else if (pTrade->Direction == '1')//������������ƽ�֣��൥�ĳֲ���ϸ
		{
			pos = &positionDetailList_NotClosed_Long;
		}
		//���Ϊ�գ�û�гֲ���ϸ���ݿ��Թ�����ɾ��
		if (!pos)
			return false;


		int   j = 0;//�������ܷ��������Ĳ�λ����
		bool findflag = false;//��־���Ƿ��ҵ���һ�ʷ��������ĳֲ���ϸ
		
							  //�Ƿ���ƽ��
		bool closeTd = pTrade->OffsetFlag == THOST_FTDC_OF_CloseToday && (strcmp(pTrade->ExchangeID, "SHFE") == 0 || strcmp(pTrade->ExchangeID, "INE") == 0);
		
		//------------�����ӵ�����-------------����¼���־���
		double avgOpenPrice = 0.0;
		double avgHoldingPrice = 0.0;


		//������Щ��ϸ��ƽ�ֶ�Ӧ
		for (auto it = (*pos).begin(); it != (*pos).end(); it++)

		{
			if (strcmp((*it)->InstrumentID, pTrade->InstrumentID) == 0)//�Ƿ��Ǹ�ƽ�ֺ�Լ
			{
				if (closeTd)//ƽ������Ҫ���ҳֲ���ϸ�е�һ�ʵ��տ��ֵĳֲ�
				{
					
					bool isToday = strcmp((*it)->TradeDate, m_cTradingDay) == 0 ;
					
					//�޸�
					if (!findflag && isToday)
						findflag = true;
					if (findflag && isToday)
					{
						int temp = j;
						//�жϵ�i���ֲ���ϸ�������Ƿ��㹻vol-temp
						j = temp + ((*it)->Volume > (vol - temp) ? (vol - temp) : (*it)->Volume);
						//�����ҵ�ƽ������5�֣�tempΪ0���Ƿ��Ǵ���vol - temp��temp+�����
						(*it)->Volume -= (j - temp);//�ֲ������٣�j - temp��

						//------------------����������---------,���ܿ��ֽ��
						avgOpenPrice += (double)(j - temp) * (*it)->Price;

					
					}
				}
				else//ƽ�֣�
				{
					//������������Դ����ƽ��
					if (strcmp(pTrade->ExchangeID, "SHFE") == 0 || strcmp(pTrade->ExchangeID, "INE") == 0)
					{
					//
						if (!findflag && strcmp((*it)->TradeDate, m_cTradingDay) != 0)
							findflag = true;
						if (findflag && strcmp((*it)->TradeDate, m_cTradingDay) != 0)
						{
							int temp = j;
							//�жϵ�i���ֲ���ϸ�������Ƿ��㹻vol-temp
							j = temp + ((*it)->Volume > (vol - temp) ? (vol - temp) : (*it)->Volume);
							(*it)->Volume -= (j - temp);
							//------------------����������---------,���ܿ��ֽ��
							avgOpenPrice += (double)(j - temp) * (*it)->Price;
							

						}
					}
					else//�н�������������֣����ƽ��
					{
						if (!findflag)
							findflag = true;
						if (findflag)
						{
							int temp = j;
							j = temp + ((*it)->Volume > (vol - temp) ? (vol - temp) : (*it)->Volume);
							(*it)->Volume -= (j - temp);

							//------------------����������---------,���ܿ��ֽ��
							avgOpenPrice += (double)(j - temp) * (*it)->Price;
						}
					}
				}
				if (j == vol)
					break;
			}
		}
		
		//ɾ���ֲ���Ϊ0�ĳֲ���ϸ
		for (auto it = (*pos).begin(); it != (*pos).end();)
		{
			if ((*it)->Volume == 0)
				it = (*pos).erase(it);//erase����it�����һ����������������it++��
			else
				it++;
		}

		//------------------����������---------,���㿪�־��ۡ�ƽ��ӯ��
		string strInst = pTrade->InstrumentID;
		auto it = m_position_field_map.find(strInst);
		if (it != m_position_field_map.end())
		{
			//���㿪�־���
			avgOpenPrice /= vol;
			
			int multi = 0;
			auto instIt = m_all_instruments_field_map.find(strInst);
			//����ҵ��ú�Լ
			if (instIt != m_all_instruments_field_map.end())
			{
				multi = instIt->second->VolumeMultiple;
				//����ƽ��ӯ��
				if (pTrade->Direction == '0')//���뷽��,����ƽ�֣���Ӧ���ǵĿյ�
				{
					it->second->ShortCloseProfit += (avgOpenPrice - pTrade->Price) * multi * vol;
					

				}
				else//����ƽ�֣���Ӧ���ǵĶ൥
				{
					it->second->LongCloseProfit += ( pTrade->Price - avgOpenPrice) * multi * vol;
					
				}
			}

		}
	}
	
	//avgOpenPrice += (double)(j - temp) * (*it)->Price;
	return true;
}

void TdSpi::ShowPositionDetail()
{
	std::lock_guard<std::mutex> m_lock(m_mutex);//��������֤���set���ݵİ�ȫ
	cerr << endl << endl;
	cerr << "---------------��ӡ�൥�ֲ���ϸ-------------" << endl;
	for (auto iter = positionDetailList_NotClosed_Long.begin(); iter != positionDetailList_NotClosed_Long.end(); iter++)
	{
		cerr << endl << "Ͷ���ߴ��룺" << (*iter)->InvestorID << "  "
			<< "�û����룺" << (*iter)->UserID << "  " << "�ɽ���ţ�" << (*iter)->TradeID << "  "
			<< "��Լ���룺" << (*iter)->InstrumentID << "  " << "��������" << (*iter)->Direction << "  "
			<< "��ƽ��" << (*iter)->OffsetFlag << "  " << "Ͷ��/�ױ�" << (*iter)->HedgeFlag << "  "
			<< "�۸�" << (*iter)->Price << "  " << "������" << (*iter)->Volume << "  "
			<< "�������ã�" << (*iter)->OrderRef << "  " << "���ر�����ţ�" << (*iter)->OrderLocalID << "  "
			<< "�ɽ�ʱ�䣺" << (*iter)->TradeTime << "  " << "ҵ��Ԫ��" << (*iter)->BusinessUnit << "  "
			<< "��ţ�" << (*iter)->SequenceNo << "  " << "���͹�˾�µ���ţ�" << (*iter)->BrokerOrderSeq << "  "
			<< "�����գ�" << (*iter)->TradingDay << endl;
	}

	cerr << endl << endl;

	cerr << "---------------��ӡ�յ��ֲ���ϸ-------------" << endl;
	for (auto iter = positionDetailList_NotClosed_Short.begin(); iter != positionDetailList_NotClosed_Short.end(); iter++)
	{
		cerr << endl << "Ͷ���ߴ��룺" << (*iter)->InvestorID << "  "
			<< "�û����룺" << (*iter)->UserID << "  " << "�ɽ���ţ�" << (*iter)->TradeID << "  "
			<< "��Լ���룺" << (*iter)->InstrumentID << "  " << "��������" << (*iter)->Direction << "  "
			<< "��ƽ��" << (*iter)->OffsetFlag << "  " << "Ͷ��/�ױ�" << (*iter)->HedgeFlag << "  "
			<< "�۸�" << (*iter)->Price << "  " << "������" << (*iter)->Volume << "  "
			<< "�������ã�" << (*iter)->OrderRef << "  " << "���ر�����ţ�" << (*iter)->OrderLocalID << "  "
			<< "�ɽ�ʱ�䣺" << (*iter)->TradeTime << "  " << "ҵ��Ԫ��" << (*iter)->BusinessUnit << "  "
			<< "��ţ�" << (*iter)->SequenceNo << "  " << "���͹�˾�µ���ţ�" << (*iter)->BrokerOrderSeq << "  "
			<< "�����գ�" << (*iter)->TradingDay << endl;
	}

	cerr << endl << endl;
}

void TdSpi::UpdatePositionFieldMap(TThostFtdcInstrumentIDType InstrumentID)
{
	// ���Һ�Լ�Ƿ����
	bool bFind = FindPositionInst(InstrumentID);
	if (!bFind)
	{
		InsertPositionInst(InstrumentID);
	}

}
/// <summary>
/// �ڳֲֺ�Լӳ���в���ָ���ĺ�Լ
/// </summary>
/// <param name="InstrumentID"> ָ���ĺ�Լ����</param>
/// <returns>�����Ƿ��ҵ�</returns>
bool TdSpi::FindPositionInst(TThostFtdcInstrumentIDType InstrumentID)
{
	
	bool bFindInst = false;

	for (auto it = m_position_field_map.begin(); it != m_position_field_map.end(); it++)
	{
		if (strcmp((it->first).c_str(),InstrumentID) == 0)
		{
			bFindInst = true;
			break;
		}
	}


	return bFindInst;
}

void TdSpi::InsertPositionInst(TThostFtdcInstrumentIDType InstrumentID)
{
	cerr << "-----------------------û�������Լ����Ҫ���콻����Ϣ�ṹ��" << endl;
	position_field* p_position_field = new position_field();
	p_position_field->instId = InstrumentID;
	//����ֲֺ�Լ��string
	m_Inst_Position += InstrumentID;
	m_Inst_Position += ",";
	m_position_field_map.insert(pair<string, position_field*>(InstrumentID, p_position_field));
}
/// <summary>
/// �ڶ��ĺ�Լ��subscribe_inst_vec�����в��Ҹú�Լ�Ƿ����
/// </summary>
/// <param name="InstrumentID">ָ����Լ����</param>
/// <returns>��Լ�Ƿ������������</returns>
bool TdSpi::FindInstInSubVec(TThostFtdcInstrumentIDType InstrumentID)
{
	bool findInst = false;
	for (auto it = subscribe_inst_vec.begin(); it != subscribe_inst_vec.end(); it++)
	{
		if (strcmp((*it).c_str(), InstrumentID) == 0)
		{
			findInst = true;
			break;
		}
	}

	return findInst;
}

void TdSpi::SubscribeMarketData(std::string strInst)
{
	m_pUserMDSpi_trade->SubscribeMarketData(strInst);
}

void TdSpi::UpdatePosition()
{
	//����ӯ��
	double openProfit = 0.0;
	//�ֲ�ӯ��
	double positionProfit = 0.0;
	//ƽ��ӯ��
	double closeProfit = 0.0;
	//����
	std::lock_guard<std::mutex> m_lock(m_mutex);
	for (auto it = m_position_field_map.begin(); it != m_position_field_map.end(); it++)
	{
		ReFreshPositionField(it->first,it->second);
		openProfit += it->second->LongOpenProfit + it->second->ShortOpenProfit;
		positionProfit += it->second->LongPositionProfit + it->second->ShortPositionProfit;
		closeProfit += it->second->LongCloseProfit + it->second->ShortCloseProfit;
		
	}
	m_OpenProfit = openProfit;
	m_PositionProfit = positionProfit;
	m_CloseProfit = closeProfit;
	cerr << "�˻�����ӯ�� " << m_OpenProfit << "�˻��ֲ�ӯ�� " << m_PositionProfit << "�˻�ƽ��ӯ�� " << m_CloseProfit << endl;
	

}

void TdSpi::ReFreshPositionField(std::string strInst, position_field* pf)
{
	position_field pfo;
	//�Ƿ��ܹ��ڳֲ���ϸ���ҵ��ú�Լ
	bool  findInst = false;
	//1���ڶ൥�ĳֲ���ϸ�в��ң����¶�ͷ�ֲ�������ƽ����
	for (auto it = positionDetailList_NotClosed_Long.begin(); it != positionDetailList_NotClosed_Long.end(); it++)
	{
		//�жϺ�Լ�Ƿ���ָ����Լ
		if (strcmp(strInst.c_str(), (*it)->InstrumentID) == 0)
		{
			findInst = true;
			//1�����ֲܳ�����
			pfo.LongPosition += (*it)->Volume;
			//2)���ܽ������
			if (strcmp(m_cTradingDay, (*it)->TradeDate) == 0)//�и��ӣ���Щ������ֻ�ṩ��TradeDate
				//TradeDate,������TradeDate�����˸ñʽ��׿���ʱ�ģ������գ���TradingDay����Ľ�����
				//������1��4�գ�������ҹ�̽���ʱ�䣬��ôTradeDate����1��5�գ�TradingDayΪ1��5��
			//if (strcmp(m_cTradingDay, (*it)->TradeDate) == 0 || strcmp(m_cTradingDay, (*it)->TradingDay) == 0)//�и��ӣ���Щ������ֻ�ṩ��TradeDate
			{
				pfo.TodayLongPosition+= (*it)->Volume;
				//5)���ֲֽܳ���Ӧ���ʹ�ÿ��ּۣ�������������ֲ־���
				pfo.LongAvgHoldingPrice+= (*it)->Price* (*it)->Volume;
			}
			else//3)�����������
			{
				pfo.YdLongPosition += (*it)->Volume;
				//5)���ֲֽܳ���Ӧ�ϲ�ʹ�����ۣ���������ֲ־���
				pfo.LongAvgHoldingPrice += pf->PreSettlementPrice * (*it)->Volume;
			}

			//4)���ܿ��ֽ��������㿪�־���
			pfo.LongAvgEntryPrice += (*it)->Price * (*it)->Volume;
			
		}
	}
	//�����λ��Ϊ0�����Լ�����������
	if (pfo.LongPosition > 0)
	{
		pfo.LongAvgEntryPrice /= pfo.LongPosition;
		pfo.LongAvgHoldingPrice /= pfo.LongPosition;

		//6)����ֲ�ӯ���͸��������֣�ӯ��
		//������Ҫ��Լ����
		int multi = 0;
		auto instIt = m_all_instruments_field_map.find(strInst);
		//����ҵ��ú�Լ
		if (instIt != m_all_instruments_field_map.end())
		{
			multi = instIt->second->VolumeMultiple;
			//���㸡�������֣�ӯ��
			pfo.LongOpenProfit = (pf->LastPrice - pfo.LongAvgEntryPrice) * multi * pfo.LongPosition;
			//����ֲ�ӯ��
			pfo.LongPositionProfit = (pf->LastPrice - pfo.LongAvgHoldingPrice) * multi * pfo.LongPosition;
		}
		//��������ֵ��pfָ��Ķ���
		//������λ
		pf->LongPosition = pfo.LongPosition;
		pf->TodayLongPosition = pfo.TodayLongPosition;
		pf->YdLongPosition = pfo.YdLongPosition;
		//�����۸�
		pf->LongAvgEntryPrice = pfo.LongAvgEntryPrice;
		pf->LongAvgHoldingPrice = pfo.LongAvgHoldingPrice;

		//����ӯ��
		pf->LongPositionProfit = pfo.LongPositionProfit;
		pf->LongOpenProfit = pfo.LongOpenProfit;

	}
	//pfo.LongAvgHoldingProfit
	if (!findInst)//��ͷ��λ��û�иú�Լ����Ҫ�Ѳ�λ����֡���ּ����ּۡ��ֲּ۾�����Ϊ0
	{
		//������λ
		pf->LongPosition = 0;
		pf->TodayLongPosition = 0;
		pf->YdLongPosition = 0;
		//�����۸�
		pf->LongAvgEntryPrice = 0.0;
		pf->LongAvgHoldingPrice = 0.0;

		//����ӯ��
		pf->LongPositionProfit = 0.0;
		pf->LongOpenProfit = 0.0;
	}

	findInst = false;




	//1���ڿյ��ĳֲ���ϸ�в��ң����¿�ͷ�ֲ�������ƽ����
	for (auto it = positionDetailList_NotClosed_Short.begin(); it != positionDetailList_NotClosed_Short.end(); it++)
	{
		//�жϺ�Լ�Ƿ���ָ����Լ
		if (strcmp(strInst.c_str(), (*it)->InstrumentID) == 0)
		{
			findInst = true;
			//1�����ֲܳ�����
			pfo.ShortPosition += (*it)->Volume;
			//2)���ܽ������
			if (strcmp(m_cTradingDay, (*it)->TradeDate) == 0)//�и��ӣ���Щ������ֻ�ṩ��TradeDate
				//if (strcmp(m_cTradingDay, (*it)->TradeDate) == 0 || strcmp(m_cTradingDay, (*it)->TradingDay) == 0)//�и��ӣ���Щ������ֻ�ṩ��TradeDate
			{
				pfo.TodayShortPosition += (*it)->Volume;
				//5)���ֲֽܳ���Ӧ���ʹ�ÿ��ּۣ�������������ֲ־���
				pfo.ShortAvgHoldingPrice += (*it)->Price * (*it)->Volume;
			}
			else//3)�����������
			{
				pfo.YdShortPosition += (*it)->Volume;
				//5)���ֲֽܳ���Ӧ�ϲ�ʹ�����ۣ���������ֲ־���
				pfo.ShortAvgHoldingPrice += pf->PreSettlementPrice * (*it)->Volume;
			}

			//4)���ܿ��ֽ��������㿪�־���
			pfo.ShortAvgEntryPrice += (*it)->Price * (*it)->Volume;

		}
	}
	//�����λ��Ϊ0�����Լ�����������
	if (pfo.ShortPosition > 0)
	{
		pfo.ShortAvgEntryPrice /= pfo.ShortPosition;
		pfo.ShortAvgHoldingPrice /= pfo.ShortPosition;

		//6)����ֲ�ӯ���͸��������֣�ӯ��
		//������Ҫ��Լ����
		int multi = 0;
		auto instIt = m_all_instruments_field_map.find(strInst);
		//����ҵ��ú�Լ
		if (instIt != m_all_instruments_field_map.end())
		{
			multi = instIt->second->VolumeMultiple;
			//���㸡�������֣�ӯ��
			pfo.ShortOpenProfit = (pfo.ShortAvgEntryPrice - pf->LastPrice) * multi * pfo.ShortPosition;
			//����ֲ�ӯ��
			pfo.ShortPositionProfit = (pfo.ShortAvgHoldingPrice - pf->LastPrice ) * multi * pfo.ShortPosition;
		}
		//��������ֵ��pfָ��Ķ���
		//������λ
		pf->ShortPosition = pfo.ShortPosition;
		pf->TodayShortPosition = pfo.TodayShortPosition;
		pf->YdShortPosition = pfo.YdShortPosition;
		//�����۸�
		pf->ShortAvgEntryPrice = pfo.ShortAvgEntryPrice;
		pf->ShortAvgHoldingPrice = pfo.ShortAvgHoldingPrice;

		//����ӯ��
		pf->ShortPositionProfit = pfo.ShortPositionProfit;
		pf->ShortOpenProfit = pfo.ShortOpenProfit;

	}
	//pfo.LongAvgHoldingProfit
	if (!findInst)//��ͷ��λ��û�иú�Լ����Ҫ�Ѳ�λ����֡���ּ����ּۡ��ֲּ۾�����Ϊ0
	{
		//������λ
		pf->ShortPosition = 0;
		pf->TodayShortPosition = 0;
		pf->YdShortPosition = 0;
		//�����۸�
		pf->ShortAvgEntryPrice = 0.0;
		pf->ShortAvgHoldingPrice = 0.0;

		//����ӯ��
		pf->ShortPositionProfit = 0.0;
		pf->ShortOpenProfit = 0.0;
	}





}

bool TdSpi::BeUpDatedMDPrice()
{
	//����
	std::lock_guard<std::mutex> m_lock(m_mutex);
	auto it = m_position_field_map.begin();
	while (it != m_position_field_map.end())
	{
		if (it->second->LastPrice < 0.0001 || it->second->PreSettlementPrice < 0.0001)
			break;
		it++;
	}
	//��������������������¼ۺ����۶�����
	bUpdateMdPrice = (it == m_position_field_map.end());
	return bUpdateMdPrice;
}

void TdSpi::UpdatePositionMDPrice(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	
	//���ݹ��ˣ�����һЩ�쳣����
	//if(pDepthMarketData->UpdateTime)
	if (pDepthMarketData->PreSettlementPrice < 0.001 || pDepthMarketData->LastPrice < 0.001)
		return;
	if (pDepthMarketData->LastPrice < pDepthMarketData->LowerLimitPrice || pDepthMarketData->LastPrice > pDepthMarketData->UpperLimitPrice)
		return;


	string strInst = pDepthMarketData->InstrumentID;
	//����
	std::lock_guard<std::mutex> m_lock(m_mutex);
	//��ӳ����Ѱ�Ҹú�Լ
	auto it = m_position_field_map.find(strInst);
	//�ҵ��˸ú�Լ
	if (it != m_position_field_map.end())
	{
		if(it->second->PreSettlementPrice < 0.1)
		it->second->PreSettlementPrice = pDepthMarketData->PreSettlementPrice;
		it->second->LastPrice = pDepthMarketData->LastPrice;
	}

}

void TdSpi::Release()
{
	//���orderList����������б�������
	for (auto it = orderList.begin(); it != orderList.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	orderList.clear();
	

	//���m_mOrders����������б�������
	for (auto it = m_mOrders.begin(); it != m_mOrders.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	m_mOrders.clear();
	//��������гɽ�
	//std::vector<CThostFtdcTradeField*> tradeList;
	for (auto it = tradeList.begin(); it != tradeList.end(); it++)
	{
		delete (*it);
		*it = nullptr;
	}
	tradeList.clear();

	//positionDetailList_NotClosed_Long
	for (auto it = positionDetailList_NotClosed_Long.begin(); it != positionDetailList_NotClosed_Long.end(); it++)
	{
		delete (*it);
		*it = nullptr;
	}
	positionDetailList_NotClosed_Long.clear();

	//positionDetailList_NotClosed_Short
	for (auto it = positionDetailList_NotClosed_Short.begin(); it != positionDetailList_NotClosed_Short.end(); it++)
	{
		delete (*it);
		*it = nullptr;
	}
	positionDetailList_NotClosed_Short.clear();

	//std::map<std::string, position_field*> m_position_field_map;

	for (auto it = m_position_field_map.begin(); it != m_position_field_map.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	m_position_field_map.clear();
	////���ڱ��������ڻ���Լ��ӳ��
	//std::map<std::string, CThostFtdcInstrumentField*> m_all_instruments_field_map;

	for (auto it = m_all_instruments_field_map.begin(); it != m_all_instruments_field_map.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	m_all_instruments_field_map.clear();
	
	////������ı�������
	//std::map<int, CThostFtdcOrderField*> m_Orders;
	
	for (auto it = m_Orders.begin(); it != m_Orders.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	m_Orders.clear();

	////������ĳɽ�vector
	//std::vector<CThostFtdcTradeField*> m_Trades;

	for (auto it = m_Trades.begin(); it != m_Trades.end(); it++)
	{
		delete *it;
		*it = nullptr;
	}
	m_Trades.clear();
}


CThostFtdcOrderField * TdSpi::PlaceOrder(const char* pszCode, const char* pszExchangeID, int nDirection, int nOpenClose, int nVolume, double fPrice)
{
	//���������ṹ�����
	CThostFtdcInputOrderField orderField;
	//��ʼ������
	memset(&orderField, 0, sizeof(CThostFtdcInputOrderField));

	//fill the broker and user fields;
	
	strcpy(orderField.BrokerID, m_BrokerId.c_str());//�������ǵ�brokerid���ṹ�����
	strcpy(orderField.InvestorID, m_UserId.c_str());//�������ǵ�InvestorID���ṹ�����

	//set the Symbol code;
	strcpy(orderField.InstrumentID, pszCode);//�����µ����ڻ���Լ
	//CThostFtdcInstrumentField* instField = m_all_instruments_field_map.find(pszCode)->second;
	/*if (instField)
	{
		const char* ExID = instField->ExchangeID;
		strcpy(orderField.ExchangeID, ExID);
	}*/
	strcpy(orderField.ExchangeID, pszExchangeID);//���������룬"SHFE","CFFEX","DCE","CZCE","INE"
	if (nDirection == 0) {
		orderField.Direction = THOST_FTDC_D_Buy;
	}
	else {
		orderField.Direction = THOST_FTDC_D_Sell;
	}

	orderField.LimitPrice = fPrice;//�۸�

	orderField.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;//Ͷ�������������ױ������е�

	
	
	orderField.VolumeTotalOriginal = nVolume;//�µ�����

	//--------------1���۸�����----------------
		//�޼۵���
	orderField.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//�����ļ۸���������

	//�м۵���
	//orderField.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
	//�н����м�
	//orderField.OrderPriceType = THOST_FTDC_OPT_FiveLevelPrice;

	//--------------2����������----------------
	//	///����
//#define THOST_FTDC_CC_Immediately '1'
/////ֹ��
//#define THOST_FTDC_CC_Touch '2'
/////ֹӮ
//#define THOST_FTDC_CC_TouchProfit '3'
/////Ԥ��
//#define THOST_FTDC_CC_ParkedOrder '4'
/////���¼۴���������
//#define THOST_FTDC_CC_LastPriceGreaterThanStopPrice '5'
/////���¼۴��ڵ���������
//#define THOST_FTDC_CC_LastPriceGreaterEqualStopPrice '6'
/////���¼�С��������
//#define THOST_FTDC_CC_LastPriceLesserThanStopPrice '7'
/////���¼�С�ڵ���������
//#define THOST_FTDC_CC_LastPriceLesserEqualStopPrice '8'
/////��һ�۴���������
//#define THOST_FTDC_CC_AskPriceGreaterThanStopPrice '9'
/////��һ�۴��ڵ���������
//#define THOST_FTDC_CC_AskPriceGreaterEqualStopPrice 'A'
/////��һ��С��������
//#define THOST_FTDC_CC_AskPriceLesserThanStopPrice 'B'
/////��һ��С�ڵ���������
//#define THOST_FTDC_CC_AskPriceLesserEqualStopPrice 'C'
/////��һ�۴���������
//#define THOST_FTDC_CC_BidPriceGreaterThanStopPrice 'D'
/////��һ�۴��ڵ���������
//#define THOST_FTDC_CC_BidPriceGreaterEqualStopPrice 'E'
/////��һ��С��������
//#define THOST_FTDC_CC_BidPriceLesserThanStopPrice 'F'
/////��һ��С�ڵ���������
//#define THOST_FTDC_CC_BidPriceLesserEqualStopPrice 'H'

	orderField.ContingentCondition = THOST_FTDC_CC_Immediately;//�����Ĵ�������
	//orderField.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterThanStopPrice;//�����Ĵ�������
	//orderField.StopPrice = 5035.0;//



	//--------------3��ʱ������----------------
	//orderField.TimeCondition = THOST_FTDC_TC_IOC;

//	///������ɣ�������
//#define THOST_FTDC_TC_IOC '1'
/////������Ч
//#define THOST_FTDC_TC_GFS '2'
/////������Ч
//#define THOST_FTDC_TC_GFD '3'
/////ָ������ǰ��Ч
//#define THOST_FTDC_TC_GTD '4'
/////����ǰ��Ч
//#define THOST_FTDC_TC_GTC '5'
/////���Ͼ�����Ч
//#define THOST_FTDC_TC_GFA '6'


	//orderField.TimeCondition = THOST_FTDC_TC_GFS;//ʱ������,������Ч,------û���ã��������������н���������֧�ֵı�������
	//orderField.TimeCondition = THOST_FTDC_TC_GTD;//ʱ������,ָ��������Ч,��������֧�ֵı�������
	//orderField.TimeCondition = THOST_FTDC_TC_GTC;//ʱ������,����ǰ��Ч,��������֧�ֵı�������
	//orderField.TimeCondition = THOST_FTDC_TC_GFA;//ʱ������,���Ͼ�����Ч,��������֧�ֵı�������


	//orderField.TimeCondition = THOST_FTDC_TC_IOC;//ʱ��������������ɣ�������
	orderField.TimeCondition = THOST_FTDC_TC_GFD;//ʱ������,������Ч


	//--------------4����������----------------
	orderField.VolumeCondition = THOST_FTDC_VC_AV;//��������

	//orderField.VolumeCondition = THOST_FTDC_VC_MV;//��С����
	//orderField.VolumeCondition = THOST_FTDC_VC_CV;//ȫ������


	

	strcpy(orderField.GTDDate, m_cTradingDay);//�µ�������



	orderField.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;//ǿƽԭ��

	switch (nOpenClose) {
	case 0:
		orderField.CombOffsetFlag[0] = THOST_FTDC_OF_Open;//����
		break;
	case 1:
		orderField.CombOffsetFlag[0] = THOST_FTDC_OF_Close;//ƽ��
		break;
	case 2:
		orderField.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;//ƽ���
		break;
	case 3:
		orderField.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;//ƽ���
		break;
	}

	
	//******************************�������ò���***********************************
	
	/*sprintf_s(orderRef, sizeof(orderRef), "%012d", 1);
	orderField.OrderRef*/
	GetNextOrderRef();
	strcpy(orderField.OrderRef, orderRef);//��������

	//******************************�������ò���***********************************




	//���ý��׵�api��ReqOrderInsert
	int retCode = m_pUserTDApi_trade->ReqOrderInsert(&orderField, GetNextRequestID());
	
	//�ĳ��˴����
	//int retCode = m_pUserTDApi_trade->ReqOrderInsert(&orderField, 1);

	if (retCode != 0) {
		printf("failed to insert order,instrument: %s, volume: %d, price: %f\n", pszCode, nVolume, fPrice);
	}


	//�����ӵ�����
	//CThostFtdcOrderField�ṹ��ָ�룬������ѿռ�
	CThostFtdcOrderField* pOrder = new CThostFtdcOrderField;
	//��ʼ������
	memset(pOrder, 0, sizeof(CThostFtdcOrderField));
	strcpy(pOrder->BrokerID, orderField.BrokerID);
	strcpy(pOrder->InvestorID, orderField.InvestorID);

	strcpy(pOrder->InstrumentID, orderField.InstrumentID);
	strcpy(pOrder->ExchangeID, orderField.ExchangeID);
	pOrder->Direction = orderField.Direction;

	pOrder->LimitPrice = orderField.LimitPrice;
	pOrder->CombHedgeFlag[0] = orderField.CombHedgeFlag[0];
	pOrder->VolumeTotalOriginal = orderField.VolumeTotalOriginal;


	pOrder->OrderPriceType = orderField.OrderPriceType;
	pOrder->ContingentCondition = orderField.ContingentCondition;
	pOrder->TimeCondition = orderField.TimeCondition;
	pOrder->VolumeCondition = orderField.VolumeCondition;


	strcpy(pOrder->GTDDate, orderField.GTDDate);//�µ�������
	pOrder->ForceCloseReason = orderField.ForceCloseReason;//ǿƽԭ��
	pOrder->CombOffsetFlag[0] = orderField.CombOffsetFlag[0];//��ƽ
	pOrder->FrontID = m_nFrontID;
	pOrder->SessionID = m_nSessionID;
	strcpy(pOrder->OrderRef, orderField.OrderRef);
	char str[50] = { '\0' };
	sprintf_s(str, sizeof(str), "%d%d%s", pOrder->FrontID, pOrder->SessionID, pOrder->OrderRef);
	m_mOrders.insert(make_pair(str, pOrder));
	return pOrder;

}
