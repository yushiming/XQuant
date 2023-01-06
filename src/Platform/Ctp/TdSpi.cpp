#include "TdSpi.h"
#include<map>
#include<iostream>
#include<mutex>
#include"Strategy.h"
using namespace std;


extern std::map<std::string, std::string> accountConfig_map;//保存账户信息的map
//全局的互斥锁
extern std::mutex m_mutex;
extern Strategy* g_strategy;//策略类指针

extern int g_nRequestID;

//全局的持仓合约
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

	//浮动盈亏
	m_OpenProfit = 0.0;
	//平仓盈亏
	m_CloseProfit = 0.0;
	//持仓盈亏
	m_PositionProfit = 0.0;
	
}

TdSpi::~TdSpi()
{
	Release();
}

void TdSpi::OnFrontConnected()
{
	cerr << "OnFrontConnected ：" << endl;
	static const char* version = m_pUserTDApi_trade->GetApiVersion();
	cerr << "当前的CTP Api Version：" <<version<< endl;
	ReqAuthenticate();
}



int TdSpi::ReqAuthenticate()
{
	//virtual int ReqAuthenticate(CThostFtdcReqAuthenticateField * pReqAuthenticateField, int nRequestID) = 0;
	CThostFtdcReqAuthenticateField req;
	//初始化
	memset(&req, 0, sizeof(req));


	strcpy(req.AppID, m_AppId.c_str());
	//strcpy(req.AppID, "eiruoejladkfj");
	strcpy(req.AuthCode, m_AuthCode.c_str());
	//strcpy(req.AuthCode, "eiruoejladkfj");
	strcpy(req.BrokerID, m_BrokerId.c_str());
	//strcpy(req.BrokerID,"0000");
	//strcpy(req.UserID, m_UserId.c_str());
	strcpy(req.UserID, "");
	cerr << "请求认证的账户信息：" << endl << " appid: " << req.AppID << " authcode: " << req.AuthCode
		<< " brokerid: " << req.BrokerID << " userId: " << req.UserID << endl;
	
	return m_pUserTDApi_trade->ReqAuthenticate(&req,GetNextRequestID());
}
void TdSpi::OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (pRspInfo)
	{
		if (pRspInfo->ErrorID == 0)
		{
			cerr << "穿透测试验证成功！" << "ErrMsg:" << pRspInfo->ErrorMsg << endl;
			ReqUserLogin();
		}

		else
		{
			cerr << "穿透测试验证失败！" << " errorid:" << pRspInfo->ErrorID <<
				"ErrMsg:" << pRspInfo->ErrorMsg << endl;

			//<error id = "AUTH_FAILED" value = "63" prompt = "CTP:客户端认证失败" / >
		}
	}

}
int TdSpi::ReqUserLogin()
{
	cerr << "====ReqUserLogin====,用户登录中..." <<endl;
	//定义一个CThostFtdcReqUserLoginField
	CThostFtdcReqUserLoginField reqUserLogin;
	//初始化为0
	memset(&reqUserLogin, 0, sizeof(reqUserLogin));
	//复制brokerid,userid,passwd
	strcpy(reqUserLogin.BrokerID, m_BrokerId.c_str());
	//strcpy(reqUserLogin.BrokerID, "0000");
	//errorid:3，不合法的登录
	//strcpy(reqUserLogin.UserID, "00000000");
	strcpy(reqUserLogin.UserID, m_UserId.c_str());
	strcpy(reqUserLogin.Password, m_Passwd.c_str());

	//strcpy(reqUserLogin.Password, "00000000");
	
	//登录
	return m_pUserTDApi_trade->ReqUserLogin(&reqUserLogin, GetNextRequestID());

}

void TdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "登录请求回调OnRspUserLogin" << endl;
	if (!IsErrorRspInfo(pRspInfo) && pRspUserLogin)
	{
		m_nFrontID = pRspUserLogin->FrontID;
		m_nSessionID = pRspUserLogin->SessionID;
		int nextOrderRef = atoi(pRspUserLogin->MaxOrderRef);

		sprintf_s(orderRef, sizeof(orderRef), "%012d", ++nextOrderRef);

		cout << "前置编号:" << pRspUserLogin->FrontID << endl
			<< "会话编号" << pRspUserLogin->SessionID << endl
			<< "最大报单引用:" << pRspUserLogin->MaxOrderRef << endl
			<< "上期所时间：" << pRspUserLogin->SHFETime << endl
			<< "大商所时间：" << pRspUserLogin->DCETime << endl
			<< "郑商所时间：" << pRspUserLogin->CZCETime << endl
			<< "中金所时间：" << pRspUserLogin->FFEXTime << endl
			<< "能源所时间：" << pRspUserLogin->INETime << endl
			<< "交易日：" << m_pUserTDApi_trade->GetTradingDay() << endl;
		strcpy(m_cTradingDay, m_pUserTDApi_trade->GetTradingDay());//设置交易日期
		
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
	CThostFtdcQryOrderField  QryOrderField;//定义
	memset(&QryOrderField, 0, sizeof(CThostFtdcQryOrderField));//初始化为0
	//brokerid有误
	//strcpy(QryOrderField.BrokerID, "0000");
	strcpy(QryOrderField.BrokerID, m_BrokerId.c_str());
	//InvestorID有误
	//strcpy(QryOrderField.InvestorID, "666666");
	strcpy(QryOrderField.InvestorID, m_UserId.c_str());
	//调用api的ReqQryOrder
	m_pUserTDApi_trade->ReqQryOrder(&QryOrderField, GetNextRequestID());

}

void TdSpi::OnRspQryOrder(CThostFtdcOrderField* pOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "请求查询报单响应：OnRspQryOrder" <<",pOrder  "<<pOrder<< endl;
	if (!IsErrorRspInfo(pRspInfo) && pOrder)
	{
		cerr << "请求查询报单响应：前置编号FrontID：" << pOrder->FrontID << ",会话编号:" << pOrder->SessionID
			<< ",报单引用:  " << pOrder->OrderRef << endl;
			//所有合约
		if (m_QryOrder_Once == true)
		{
			CThostFtdcOrderField* order = new CThostFtdcOrderField();
			memcpy(order, pOrder, sizeof(CThostFtdcOrderField));
			orderList.insert(make_pair(pOrder->BrokerOrderSeq, order));
			
			
			
			//bIsLast是否是最后一笔回报
			if (bIsLast)
			{
				m_QryOrder_Once = false;
				cerr << "所有合约的报单次数" << orderList.size() << endl;
				cerr << "---------------打印报单开始---------------" << endl;
				for (auto iter = orderList.begin(); iter != orderList.end(); iter++)
				{
				/*	cerr << "经纪公司代码：" << (iter->second)->BrokerID << "  " << "投资者代码：" << (iter->second)->InvestorID << "  "
						<< "用户代码：" << (iter->second)->UserID << "  " << "合约代码：" << (iter->second)->InstrumentID << "  "
						<< "买卖方向：" << (iter->second)->Direction << "  " << "组合开平标志：" << (iter->second)->CombOffsetFlag << "  "
						<< "价格：" << (iter->second)->LimitPrice << "  " << "数量：" << (iter->second)->VolumeTotalOriginal << "  "
						<< "报单引用：" << (iter->second)->OrderRef << "  " << "客户代码：" << (iter->second)->ClientID << "  "
						<< "报单状态：" << (iter->second)->OrderStatus << "  " << "委托时间：" << (iter->second)->InsertTime << "  "
						<< "报单编号：" << (iter->second)->OrderStatus << "  " << "交易日：" << (iter->second)->TradingDay << "  "
						<< "报单日期：" << (iter->second)->InsertDate << endl;*/

				}
				cerr << "---------------打印报单完成---------------" << endl;
				cerr << "查询报单正常，将首次查询成交" << endl;
				

			}
		}

	}
	else//查询出错
	{
		m_QryOrder_Once = false;
		cerr << "查询报单出错，或没有成交，将首次查询成交" << endl;
		
	}
	if (bIsLast)
	{
		//线程休眠3秒，让ctp柜台有充足的响应时间，然后再进行查询操作
		std::chrono::milliseconds sleepDuration(3 * 1000);
		std::this_thread::sleep_for(sleepDuration);
		ReqQryTrade();
	}
}


void TdSpi::ReqQryTrade()
{
	CThostFtdcQryTradeField tdField;//定义
	memset(&tdField, 0, sizeof(tdField));//初始化

	strcpy(tdField.BrokerID, m_BrokerId.c_str());
	//strcpy(tdField.BrokerID,"0000");
	strcpy(tdField.InvestorID, m_UserId.c_str());
	//strcpy(tdField.InvestorID, "888888");
	//调用交易api的ReqQryTrade
	m_pUserTDApi_trade->ReqQryTrade(&tdField, GetNextRequestID());
}
void TdSpi::OnRspQryTrade(CThostFtdcTradeField* pTrade, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{

	cerr << "请求查询成交回报响应：OnRspQryTrade" << " pTrade " << pTrade << endl;

	if (!IsErrorRspInfo(pRspInfo) && pTrade)
	{
		//所有合约
		if (m_QryTrade_Once == true)
		{
			CThostFtdcTradeField* trade = new CThostFtdcTradeField();//创建trade
			memcpy(trade, pTrade, sizeof(CThostFtdcTradeField));//pTrade复制给trade
			tradeList.push_back(trade);

			if (bIsLast)
			{
				m_QryTrade_Once = false;
				cerr << "所有合约的成交次数" << tradeList.size() << endl;
				cerr << "---------------打印成交开始---------------" << endl;
				for (vector<CThostFtdcTradeField*>::iterator iter = tradeList.begin(); iter != tradeList.end(); iter++)
				{
					cerr << "经纪公司代码：" << (*iter)->BrokerID << endl << "投资者代码：" << (*iter)->InvestorID << endl
						<< "用户代码：" << (*iter)->UserID << endl << "成交编号：" << (*iter)->TradeID << endl
						<< "合约代码：" << (*iter)->InstrumentID << endl << "买卖方向：" << (*iter)->Direction << endl
						<< "组合开平标志：" << (*iter)->OffsetFlag << endl << "投机套保标志：" << (*iter)->HedgeFlag << endl
						<< "价格：" << (*iter)->Price << endl << "数量：" << (*iter)->Volume << endl
						<< "报单引用：" << (*iter)->OrderRef << endl << "本地报单编号：" << (*iter)->OrderLocalID << endl
						<< "成交时间：" << (*iter)->TradeTime << endl << "业务单元：" << (*iter)->BusinessUnit << endl
						<< "序号：" << (*iter)->SequenceNo << endl << "经纪公司下单序号：" << (*iter)->BrokerOrderSeq << endl
						<< "交易日：" << (*iter)->TradingDay << endl;

				}
				cerr << "---------------打印成交完成---------------" << endl;
				cerr << "查询报单正常，将首次查询持仓明细" << endl;
				
			}
			

		}

	}
	else//查询出错
	{
		m_QryOrder_Once = false;
		cerr << "查询报单出错，或没有成交，将首次查询成交" << endl;
		
	}
	if (bIsLast)
	{
		//线程休眠3秒，让ctp柜台有充足的响应时间，然后再进行查询操作
		std::chrono::milliseconds sleepDuration(3 * 1000);
		std::this_thread::sleep_for(sleepDuration);
		ReqQryInvestorPositionDetail();
	}

	
}
void TdSpi::ReqQryInvestorPositionDetail()
{
	CThostFtdcQryInvestorPositionDetailField pdField;//创建
	memset(&pdField, 0, sizeof(pdField));//初始化为0
	strcpy(pdField.BrokerID, m_BrokerId.c_str());
	//strcpy(pdField.BrokerID, "0000");
	//strcpy(pdField.InstrumentID, m_InstId.c_str());


	strcpy(pdField.InvestorID, m_UserId.c_str());

	//strcpy(pdField.InvestorID, "0000");
	//调用交易api的ReqQryInvestorPositionDetail
	m_pUserTDApi_trade->ReqQryInvestorPositionDetail(&pdField,GetNextRequestID());
}

void TdSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField* pField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "请求查询投资者持仓明细回报响应：OnRspQryInvestorPositionDetail" << " pInvestorPositionDetail " << pField << endl;
	if (!IsErrorRspInfo(pRspInfo) && pField)
	{
		//所有合约
		if (m_QryDetail_Once == true)
		{
			//对于所有合约，只保存未平仓的，不保存已经平仓的
			//将程序启动前的持仓记录保存到未平仓容器tradeList_NotClosed_Long和tradeList_NotClosed_Short
			//使用结构体CThostFtdcTradeField，因为它有时间字段，而CThostFtdcInvestorPositionDetailField没有时间字段
			CThostFtdcTradeField* trade = new CThostFtdcTradeField();//创建CThostFtdcTradeField *
			
			strcpy(trade->InvestorID, pField->InvestorID);///投资者代码
			strcpy(trade->InstrumentID, pField->InstrumentID);///合约代码
			strcpy(trade->ExchangeID, pField->ExchangeID);///交易所代码
			trade->Direction = pField->Direction;//买卖方向
			trade->Price = pField->OpenPrice;//价格
			trade->Volume = pField->Volume;//数量
			strcpy(trade->TradeDate, pField->OpenDate);//成交日期
			//新增加，TradingDay这个字段，要加上
			strcpy(trade->TradingDay, pField->TradingDay);//交易日，中金所是今天，即使这个持仓明细是昨仓
			strcpy(trade->TradeID, pField->TradeID);//*********成交编号********
			if (pField->Volume > 0)//筛选未平仓合约
			{
				if (trade->Direction == '0')//买入方向
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
					
				else if (trade->Direction == '1')//卖出方向
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
			//收集持仓合约的代码
			bool find_instId = false;
			for (unsigned int i = 0; i < subscribe_inst_vec.size(); i++)
			{
				if (strcmp(subscribe_inst_vec[i].c_str(), trade->InstrumentID) == 0)//合约已存在，已订阅
				{
					find_instId = true;
					break;
				}
			}
			if (!find_instId)//合约未订阅过
			{
				cerr << "---------------------------------------该持仓合约未订阅过，加入订阅列表" << endl;
				subscribe_inst_vec.push_back(trade->InstrumentID);
			}

		}
		//输出所有合约的持仓明细，要在这边进行下一步的查询ReqQryTradingAccount()
		if (bIsLast)
		{
			m_QryDetail_Once = false;
			//持仓的合约
			string inst_holding;
			//
			for (unsigned int i = 0; i < subscribe_inst_vec.size(); i++)
				inst_holding = inst_holding + subscribe_inst_vec[i] + ",";
			//"IF2102,IF2103,"

			inst_holding = inst_holding.substr(0, inst_holding.length() - 1);//去掉最后的逗号，从位置0开始，选取length-1个字符
			//"IF2102,IF2103"

			cerr << "程序启动前的持仓列表:" << inst_holding << ",inst_holding.length()=" << inst_holding.length()
				<< ",subscribe_inst_vec.size()=" << subscribe_inst_vec.size() << endl;

			if (inst_holding.length() > 0)
				m_pUserMDSpi_trade->setInstIdList_Position_MD(inst_holding);//设置程序启动前的留仓，即需要订阅行情的合约

			//size代表笔数，而不是手数
			cerr << "账户所有合约未平仓单笔数（下单笔数，一笔可以对应多手）,多单:" << positionDetailList_NotClosed_Long.size()
				<< "空单：" << positionDetailList_NotClosed_Short.size() << endl;

			//positionDetailList_NotClosed_Long.sort(CThostFtdcTradeField::compareRule);
			cerr << "-----------------------------------------未平仓多单明细打印start" << endl;
			for (auto iter = positionDetailList_NotClosed_Long.begin(); iter != positionDetailList_NotClosed_Long.end(); iter++)
			{
				cerr << "BrokerID:" << (*iter)->BrokerID << "  " << "InvestorID:" << (*iter)->InvestorID << "  "
					<< "InstrumentID:" << (*iter)->InstrumentID << "  " << "Direction:" << (*iter)->Direction << "  "
					<< "OpenPrice:" << (*iter)->Price << "  " << "Volume:" << (*iter)->Volume << "  "
					<< "TradeDate:" << (*iter)->TradeDate << "  " << "TradeID:" << (*iter)->TradeID << endl;
			}

			cerr << "-----------------------------------------未平仓空单明细打印start" << endl;
			for (auto iter = positionDetailList_NotClosed_Short.begin(); iter != positionDetailList_NotClosed_Short.end(); iter++)
			{
				cerr << "BrokerID:" << (*iter)->BrokerID << "  " << "InvestorID:" << (*iter)->InvestorID << "  "
					<< "InstrumentID:" << (*iter)->InstrumentID << "  " << "Direction:" << (*iter)->Direction << "  "
					<< "OpenPrice:" << (*iter)->Price << "  " << "Volume:" << (*iter)->Volume << "  "
					<< "TradeDate:" << (*iter)->TradeDate << "  " << "TradeID:" << (*iter)->TradeID << endl;
			}
			cerr << "---------------打印持仓明细完成---------------" << endl;
			cerr << "查询持仓明细正常，将首次查询账户资金信息" << endl;
		}
		
	}
	else
	{
		if (m_QryDetail_Once == true)
		{
			m_QryDetail_Once = false;
			cerr << "查询持仓明细出错，或没有持仓明细，将首次查询账户资金" << endl;
		}
	}
	if (bIsLast)
	{
		//线程休眠3秒，让ctp柜台有充足的响应时间，然后再进行查询操作
		std::chrono::milliseconds sleepDuration(3 * 1000);
		std::this_thread::sleep_for(sleepDuration);
		ReqQryTradingAccount();
	}
	
}

void TdSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;//创建req的结构体对象
	memset(&req, 0, sizeof(req));//初始化
	//错误的brokerID
	//strcpy(req.BrokerID, "8888");


	strcpy(req.BrokerID, m_BrokerId.c_str());


	strcpy(req.InvestorID, m_UserId.c_str());
	//strcpy(req.InvestorID, "666666");
	//调用交易api的ReqQryTradingAccount
	int iResult = m_pUserTDApi_trade->ReqQryTradingAccount(&req, GetNextRequestID());
	cerr << "--->>> 请求查询资金账户: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void TdSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField* pTradingAccount, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "请求查询投资者资金账户回报响应：OnRspQryTradingAccount" << " pTradingAccount " << pTradingAccount << endl;
	if (!IsErrorRspInfo(pRspInfo) && pTradingAccount)
	{

		cerr << "投资者编号：" << pTradingAccount->AccountID
			<< "静态权益：期初权益" << pTradingAccount->PreBalance
			<< "动态权益：期货结算准备金" << pTradingAccount->Balance
			<< "可用资金：" << pTradingAccount->Available
			<< "可取资金：" << pTradingAccount->WithdrawQuota
			<< "当前保证金总额：" << pTradingAccount->CurrMargin
			<< "平仓盈亏：" << pTradingAccount->CloseProfit
			<< "持仓盈亏：" << pTradingAccount->PositionProfit
			<< "手续费：" << pTradingAccount->Commission
			<< "冻结保证金：" << pTradingAccount->FrozenCash
			<< endl;
		//所有合约
		if (m_QryTradingAccount_Once == true)
		{
			m_QryTradingAccount_Once = false;
		}

		cerr << "---------------打印资金账户明细完成---------------" << endl;
		cerr << "查询资金账户正常，将首次查询投资者持仓信息" << endl;
	}
	else
	{
		if (m_QryTradingAccount_Once == true)
		{
			m_QryTradingAccount_Once = false;
			cerr << "查询资金账户出错，将首次查询投资者持仓" << endl;
		}
	}
	//线程休眠3秒，让ctp柜台有充足的响应时间，然后再进行查询操作
	std::chrono::milliseconds sleepDuration(3 * 1000);
	std::this_thread::sleep_for(sleepDuration);
	ReqQryInvestorPosition_All();
}
//这个只有在自己下单的程序中才会有回调，在第三方的客户端中看不到
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
	CThostFtdcQryInvestorPositionField req;//创建req
	memset(&req, 0, sizeof(req));//初始化为0

	//strcpy(req.BrokerID, "8888");
	strcpy(req.BrokerID, m_BrokerId.c_str());
	strcpy(req.InvestorID, m_UserId.c_str());
	//strcpy(req.InvestorID, "0000");
	//合约为空，则代表查询所有合约的持仓，这个和req为空是一样的
	//strcpy(req.InstrumentID, m_InstId.c_str());
	//调用交易api的ReqQryInvestorPosition
	int iResult = m_pUserTDApi_trade->ReqQryInvestorPosition(&req, GetNextRequestID());//req为空，代表查询所有合约的持仓
	cerr << "--->>> 请求查询投资者持仓: " << ((iResult == 0) ? "成功" : "失败") << endl;
}
void TdSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField* pInvestorPosition,
	CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {
	//cerr << "请求查询持仓响应：OnRspQryInvestorPosition " << ",pInvestorPosition  " << pInvestorPosition << endl;
	if (!IsErrorRspInfo(pRspInfo) && pInvestorPosition)
	{

		//账户下所有合约
		if (m_QryPosition_Once == true)
		{
			cerr << "请求查询持仓响应：OnRspQryInvestorPosition " << " pInvestorPosition:  "
				<< pInvestorPosition << endl;//会包括已经平仓没有持仓的记录
			cerr << "响应  | 合约 " << pInvestorPosition->InstrumentID << endl
				<< " 持仓多空方向 " << pInvestorPosition->PosiDirection << endl//2多3空
			   // << " 映射后的方向 " << MapDirection(pInvestorPosition->PosiDirection-2,false) << endl
				<< " 总持仓 " << pInvestorPosition->Position << endl
				<< " 今日持仓 " << pInvestorPosition->TodayPosition << endl
				<< " 上日持仓 " << pInvestorPosition->YdPosition << endl
				<< " 保证金 " << pInvestorPosition->UseMargin << endl
				<< " 持仓成本 " << pInvestorPosition->PositionCost << endl
				<< " 开仓量 " << pInvestorPosition->OpenVolume << endl
				<< " 平仓量 " << pInvestorPosition->CloseVolume << endl
				<< " 持仓日期 " << pInvestorPosition->TradingDay << endl
				<< " 平仓盈亏（按昨结） " << pInvestorPosition->CloseProfitByDate << endl
				<< " 持仓盈亏 " << pInvestorPosition->PositionProfit << endl
				<< " 逐日盯市平仓盈亏（按昨结） " << pInvestorPosition->CloseProfitByDate << endl//快期中显示的是这个值
				<< " 逐笔对冲平仓盈亏（按开平合约） " << pInvestorPosition->CloseProfitByTrade << endl//在交易中比较有意义
				<< endl;


			//构造合约对应持仓明细信息的结构体map
			bool  find_trade_message_map = false;
			for (map<string, position_field*>::iterator iter = m_position_field_map.begin(); iter != m_position_field_map.end(); iter++)
			{
				if (strcmp((iter->first).c_str(), pInvestorPosition->InstrumentID) == 0)//合约已存在
				{
					find_trade_message_map = true;
					break;
				}
			}
			if (!find_trade_message_map)//合约不存在
			{
				cerr << "-----------------------没有这个合约，需要构造交易信息结构体" << endl;
				position_field* p_trade_message = new position_field();
				p_trade_message->instId = pInvestorPosition->InstrumentID;
				//构造持仓合约的string
				m_Inst_Position += pInvestorPosition->InstrumentID ;
				m_Inst_Position += ",";
				m_position_field_map.insert(pair<string, position_field*>(pInvestorPosition->InstrumentID, p_trade_message));
			}
			if (pInvestorPosition->PosiDirection == '2')//多单
			{
				//昨仓和今仓一次返回
				//获取该合约的持仓明细信息结构体 second; m_map[键]
				position_field* p_tdm = m_position_field_map[pInvestorPosition->InstrumentID];
				p_tdm->LongPosition = p_tdm->LongPosition + pInvestorPosition->Position;
				p_tdm->TodayLongPosition = p_tdm->TodayLongPosition + pInvestorPosition->TodayPosition;
				p_tdm->YdLongPosition = p_tdm->LongPosition - p_tdm->TodayLongPosition;
				p_tdm->LongCloseProfit = p_tdm->LongCloseProfit + pInvestorPosition->CloseProfit;
				p_tdm->LongPositionProfit = p_tdm->LongPositionProfit + pInvestorPosition->PositionProfit;
			}
			else if (pInvestorPosition->PosiDirection == '3')//空单
			{
				//昨仓和今仓一次返回

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
					cerr << "合约代码：" << iter->second->instId << endl
						<< "多单持仓量：" << iter->second->LongPosition << endl
						<< "空单持仓量：" << iter->second->ShortPosition << endl
						<< "多单今日持仓：" << iter->second->TodayLongPosition << endl
						<< "多单上日持仓：" << iter->second->YdLongPosition << endl
						<< "空单今日持仓：" << iter->second->TodayShortPosition << endl
						<< "空单上日持仓：" << iter->second->YdShortPosition << endl
						<< "多单浮动盈亏：" << iter->second->LongPositionProfit << endl
						<< "多单平仓盈亏：" << iter->second->LongCloseProfit << endl
						<< "空单浮动盈亏：" << iter->second->ShortPositionProfit << endl
						<< "空单平仓盈亏：" << iter->second->ShortCloseProfit << endl;

					//账户平仓盈亏
					m_CloseProfit = m_CloseProfit + iter->second->LongCloseProfit + iter->second->ShortCloseProfit;
					//账户浮动盈亏
					m_OpenProfit = m_OpenProfit + iter->second->LongOpenProfit + iter->second->ShortOpenProfit;

					//账户的持仓盈亏
					m_PositionProfit = m_PositionProfit + iter->second->LongPositionProfit + iter->second->ShortPositionProfit;
				}

				cerr << "账户浮动盈亏 " << m_OpenProfit << endl;
				cerr << "账户平仓盈亏 " << m_CloseProfit << endl;
			}//bisLast


		}
		cerr << "---------------查询投资者持仓完成---------------" << endl;
		cerr << "查询持仓正常，首次查询所有合约代码" << endl;
	}
	else
	{
		if (m_QryPosition_Once == true)
			m_QryPosition_Once = false;
		cerr << "查询投资者持仓出错，或没有持仓，首次查询所有合约" << endl;
	}
	if (bIsLast)
	{
		//线程休眠3秒，让ctp柜台有充足的响应时间，然后再进行查询操作
		std::chrono::milliseconds sleepDuration(10 * 1000);
		std::this_thread::sleep_for(sleepDuration);
		ReqQryInstrumetAll();
	}
	
}

/// <summary>
/// 查询单个期货合约
/// </summary>
void TdSpi::ReqQryInvestorPosition(char * pInstrument)
{
	CThostFtdcQryInvestorPositionField req;//创建req
	memset(&req, 0, sizeof(req));//初始化为0

	
	strcpy(req.BrokerID, m_BrokerId.c_str());
	strcpy(req.InvestorID, m_UserId.c_str());
	
	//合约填写具体的合约代码
	strcpy(req.InstrumentID, pInstrument);
	//调用交易api的ReqQryInvestorPosition
	int iResult = m_pUserTDApi_trade->ReqQryInvestorPosition(&req, GetNextRequestID());//req为空，代表查询所有合约的持仓
	cerr << "--->>> 请求查询投资者持仓: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void TdSpi::ReqQryInstrumetAll()
{
	CThostFtdcQryInstrumentField req;//创建req
	memset(&req, 0, sizeof(req));//初始化为0


	//调用交易api的ReqQryInstrument
	int iResult = m_pUserTDApi_trade->ReqQryInstrument(&req, GetNextRequestID());//req结构体为0，查询所有合约
	cerr << "--->>> 请求查询合约: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void TdSpi::ReqQryInstrumet(char * pInstrument)
{
	CThostFtdcQryInstrumentField req;//创建req
	memset(&req, 0, sizeof(req));//初始化为0
	strcpy(req.InstrumentID, pInstrument);//合约填写具体的代码，表示查询该合约的信息
	//调用交易api的ReqQryInstrument
	int iResult = m_pUserTDApi_trade->ReqQryInstrument(&req, GetNextRequestID());//
	cerr << "--->>> 请求查询合约: " << ((iResult == 0) ? "成功" : "失败") << endl;
}



void TdSpi::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << "请求查询合约响应：OnRspQryInstrument" << ",pInstrument   " << pInstrument->InstrumentID << endl;
	if (!IsErrorRspInfo(pRspInfo) && pInstrument)
	{

		//账户下所有合约
		if (m_QryInstrument_Once == true)
		{
			m_Instrument_All = m_Instrument_All + pInstrument->InstrumentID + ",";

			if (pInstrument->ProductClass == '1')
				//(pInstrument->ProductClass == '1' || pInstrument->ProductClass == '3')
			{
				//保存所有合约信息到map
				CThostFtdcInstrumentField* pInstField = new CThostFtdcInstrumentField();
				memcpy(pInstField, pInstrument, sizeof(CThostFtdcInstrumentField));
				m_all_instruments_field_map.insert(pair<string, CThostFtdcInstrumentField*>(pInstrument->InstrumentID, pInstField));
			}
			//策略交易的合约
			if (strcmp(m_InstId.c_str(), pInstrument->InstrumentID) == 0)
			{
				cerr << "响应 | 合约：" << pInstrument->InstrumentID
					<< "合约名称：" << pInstrument->InstrumentName
					<< " 合约在交易所代码：" << pInstrument->ExchangeInstID
					<< " 产品代码：" << pInstrument->ProductID
					<< " 产品类型：" << pInstrument->ProductClass
					<< " 多头保证金率：" << pInstrument->LongMarginRatio
					<< " 空头保证金率：" << pInstrument->ShortMarginRatio
					<< " 合约数量乘数：" << pInstrument->VolumeMultiple
					<< " 最小变动价位：" << pInstrument->PriceTick
					<< " 交易所代码：" << pInstrument->ExchangeID
					<< " 交割年份：" << pInstrument->DeliveryYear
					<< " 交割月：" << pInstrument->DeliveryMonth
					<< " 创建日：" << pInstrument->CreateDate
					<< " 到期日：" << pInstrument->ExpireDate
					<< " 上市日：" << pInstrument->OpenDate
					<< " 开始交割日：" << pInstrument->StartDelivDate
					<< " 结束交割日：" << pInstrument->EndDelivDate
					<< " 合约生命周期状态：" << pInstrument->InstLifePhase
					<< " 当前是否交易：" << pInstrument->IsTrading << endl;
			}

			if (bIsLast)
			{
				m_QryInstrument_Once = false;
				m_Instrument_All = m_Instrument_All.substr(0, m_Instrument_All.length() - 1);
				cerr << "m_Instrument_All的大小：" << m_Instrument_All.size() << endl;
				cerr << "map的大小（合约数量）：" << m_all_instruments_field_map.size() << endl;

				//将持仓合约信息设置到mdspi
				//m_pUserMDSpi_trade->setInstIdList_Position_MD(m_Inst_Postion);


				//将合约信息结构体的map复制到策略类
				g_strategy->set_instPostion_map_stgy(m_all_instruments_field_map);
				cerr << "--------------------------输出合约信息map的内容-----------------------" << endl;
				//ShowInstMessage();
				//保存全市场合约，在TD进行，需要订阅全市场合约行情时再运行
				m_pUserMDSpi_trade->set_InstIdList_All(m_Instrument_All);
				cerr << "TD初始化完成，启动MD" << endl;
				m_pUserMDApi_trade->Init();
			}
		}
	}
	else
	{
		m_QryInstrument_Once = false;
		cerr << "查询合约失败" << endl;
	}
	

}


void TdSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;//定义
	memset(&req, 0, sizeof(req));//初始化
	strcpy(req.BrokerID, m_BrokerId.c_str());
	//strcpy(req.BrokerID, "0000");
	strcpy(req.InvestorID, m_UserId.c_str());
	//strcpy(req.InvestorID, "000000");
	
	int iResult = m_pUserTDApi_trade->ReqSettlementInfoConfirm(&req, GetNextRequestID());
	cerr << "--->>> 投资者结算结果确认: " << ((iResult == 0) ? "成功" : "失败") << endl;
}


void TdSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	
	if (bIsLast && !IsErrorRspInfo(pRspInfo) && pSettlementInfoConfirm)
	{
		cerr << "响应 | 结算单..." << pSettlementInfoConfirm->InvestorID
			<< "..." << pSettlementInfoConfirm->ConfirmDate << "," <<
			pSettlementInfoConfirm->ConfirmTime << "...确认" << endl << endl;

		cerr << "结算单确认正常，首次查询报单" << endl;
		//线程休眠3秒，让ctp柜台有充足的响应时间，然后再进行查询操作
		std::chrono::milliseconds sleepDuration(3 * 1000);
		std::this_thread::sleep_for(sleepDuration);
		//Sleep(1000);
		ReqQryOrder();
	}
}

bool TdSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
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
		strcpy(pszStatus, "全部成交");
		break;
	case THOST_FTDC_OST_PartTradedQueueing:
		strcpy(pszStatus, "部分成交");
		break;
	case THOST_FTDC_OST_NoTradeQueueing:
		strcpy(pszStatus, "未成交");
		break;
	case THOST_FTDC_OST_Canceled:
		strcpy(pszStatus, "已撤单");
		break;
	case THOST_FTDC_OST_Unknown:
		strcpy(pszStatus, "未知");
		break;
	case THOST_FTDC_OST_NotTouched:
		strcpy(pszStatus, "未触发");
		break;
	case THOST_FTDC_OST_Touched:

		strcpy(pszStatus, "已触发");
		break;
	default:
		strcpy(pszStatus, "");
		break;
	}
	
	cerr << "BrokerOrderSeq:" << pOrder->BrokerOrderSeq << "  ,OrderRef;" << pOrder->OrderRef << " ,报单状态  "
		<< pszStatus << ",开平标志  " << pOrder->CombOffsetFlag[0] <<
		",报单提交状态  " << pOrder->OrderSubmitStatus <<
		",报单状态信息  " << pOrder->StatusMsg << endl << endl;
	
	
	//一、处理全部的报单
	//1、首先保存并更新这个报单
	UpdateOrder(pOrder, orderList);
	

	//2、打印所有的报单

	//ShowOrderList(orderList);

	
	//二、处理本程序的报单
	//判断是否本程序发出的报单；

	if (pOrder->FrontID != m_nFrontID || pOrder->SessionID != m_nSessionID) {

		CThostFtdcOrderField* pOld = GetOrder(pOrder->BrokerOrderSeq);
		//如果是断线重连，本程序发出的单子，会在m_Orders记录中,pOld不为空
		if (pOld == NULL) {
			return;
		}
		
	}

	
	//1、首先保存并更新这个报单
	UpdateOrder(pOrder, m_Orders);

	//2、把m_Orders打印出来
	//ShowOrderList(m_Orders);

	g_strategy->OnRtnOrder(pOrder);
	
	
	//*********************************撤单测试代码*************************
	//已经成交或已经撤单的单子，再撤一次，看看是否会报错
	/*if (pOrder->OrderStatus == '0' || pOrder->OrderStatus == '5')
	{
		CThostFtdcOrderField* pOld = GetOrder(pOrder->BrokerOrderSeq);
		CancelOrder(pOrder);
	}*/
	//*********************************撤单测试代码*************************



	///*printf("order returned,ins: %s, vol: %d, price:%f, orderref:%s,requestid:%d,traded vol: %d,ExchID:%s, OrderSysID:%s,status: %s,statusmsg:%s\n"
	//	, pOrder->InstrumentID, pOrder->VolumeTotalOriginal, pOrder->LimitPrice, pOrder->OrderRef, pOrder->RequestID
	//	, pOrder->VolumeTraded, pOrder->ExchangeID, pOrder->OrderSysID, pszStatus, pOrder->StatusMsg);*/
	////保存并更新报单的状态
	//UpdateOrder(pOrder);


	/*cerr <<"BrokerOrderSeq:"<< pOrder->BrokerOrderSeq<< "  ,OrderRef;" <<pOrder->OrderRef<<" ,报单状态  "
		<< pszStatus <<",开平标志  "<<pOrder->CombOffsetFlag[0]<< 
		",报单提交状态  "<< pOrder->OrderSubmitStatus <<
		",报单状态信息  " << pOrder->StatusMsg << endl <<endl;*/

	//if (pOrder->OrderStatus == '3'|| pOrder->OrderStatus == '1')
	//{
	//	CThostFtdcOrderField* pOld = GetOrder(pOrder->BrokerOrderSeq);
	//	if (pOld && pOld->OrderStatus != '6')
	//	{
	//		cerr << "onRtnOrder 准备撤单了:" << endl;
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
	
	//一、更新合约信息结构体的映射
	UpdatePositionFieldMap(pTrade->InstrumentID);
	
	
	//二、保存成交到tradeList，账户所有的成交列表
	//1、需要判断是否是断线重连的
	bool bFind = false;
	//1）判断该笔交易是否存在
	bFind=FindTrade(pTrade);

	//2）如果没有记录，则插入成交数据，编写一个插入成交的函数
	if (!bFind)
	{
		InsertTrade(pTrade);
		ShowTradeList();
	}
	else//3）成交记录已经存在，则返回，不处理
		return;
		
	//三、更新持仓明细
	//1、用这笔交易记录去更新我们的持仓明细
	UpdatePositionDetail(pTrade);
		
	//2、打印持仓明细
	//ShowPositionDetail();
	
	//四、更新账户持仓信息
	//1、判断合约是否订阅
	bool findInst = FindInstInSubVec(pTrade->InstrumentID);

	//2、订阅合约
	if (!findInst)
	{
		string strInst = pTrade->InstrumentID;
		subscribe_inst_vec.push_back(strInst);
		SubscribeMarketData(strInst);
	}
	//3、更新持仓信息
	UpdatePosition();

	//4、打印持仓信息
	if(bUpdateMdPrice=BeUpDatedMDPrice())
	//ShowPosition();

	//***************************************************更新本程序的成交信息*************************************
	
	//1、判断是否断线重传；
	//如果程序断线重连以后，成交会再次刷新一次
	if (FindTrade(pTrade,m_Trades))
		//如果成交记录存在，则返回，不处理
		return;

	//2、判断是否本程序发出的报单；
	CThostFtdcOrderField* pOrder = GetOrder(pTrade->BrokerOrderSeq);
	if (pOrder != NULL) {
		//	//只处理本程序发出的报单；
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
	CThostFtdcInputOrderActionField oa;//创建一个撤单的结构体对象
	memset(&oa, 0, sizeof(CThostFtdcInputOrderActionField));//初始化，字段清零
	
	oa.ActionFlag = THOST_FTDC_AF_Delete;//撤单
	//下面这三个字段，能确定我们的报单
	oa.FrontID = pOrder->FrontID;//前置编号
	oa.SessionID = pOrder->SessionID;//会话
	strcpy(oa.OrderRef, pOrder->OrderRef);//报单引用

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
	//调用交易api的撤单函数
	int nRetCode = m_pUserTDApi_trade->ReqOrderAction(&oa, oa.RequestID);

	char* pszStatus = new char[13];
	switch (pOrder->OrderStatus) {
	case THOST_FTDC_OST_AllTraded:
		strcpy(pszStatus, "全部成交");
		break;
	case THOST_FTDC_OST_PartTradedQueueing:
		strcpy(pszStatus, "部分成交");
		break;
	case THOST_FTDC_OST_NoTradeQueueing:
		strcpy(pszStatus, "未成交");
		break;
	case THOST_FTDC_OST_Canceled:
		strcpy(pszStatus, "已撤单");
		break;
	case THOST_FTDC_OST_Unknown:
		strcpy(pszStatus, "未知");
		break;
	case THOST_FTDC_OST_NotTouched:
		strcpy(pszStatus, "未触发");
		break;
	case THOST_FTDC_OST_Touched:

		strcpy(pszStatus, "已触发");
		break;
	default:
		strcpy(pszStatus, "");
		break;
	}
	/*printf("撤单ing,ins: %s, vol: %d, price:%f, orderref:%s,requestid:%d,traded vol: %d,ExchID:%s, OrderSysID:%s,status: %s,statusmsg:%s\n"
		, pOrder->InstrumentID, pOrder->VolumeTotalOriginal, pOrder->LimitPrice, pOrder->OrderRef, pOrder->RequestID
		, pOrder->VolumeTraded, pOrder->ExchangeID, pOrder->OrderSysID, pszStatus, pOrder->StatusMsg);*/
	//cerr << "TdSpi::CancelOrder 撤单ing" << pszStatus << endl;
	if (nRetCode != 0) {
		printf("cancel order failed.\n");
	}
	else
	{
		if (pOrder->OrderStatus != '0' && pOrder->OrderStatus != '5')
		{
			pOrder->OrderStatus = '6';//‘6’表示撤单途中
			UpdateOrder(pOrder);
			cerr << "TdSpi::CancelOrder 撤单ing" << pszStatus << endl;
			cerr << "TdSpi::CancelOrder 状态改为 pOrder->OrderStatus :" << pOrder->OrderStatus << endl;
		}
	}
	
	
}

void TdSpi::ShowPosition()
{
	cerr << endl << endl<<endl;
	cerr << "打印账户持仓------------------------------------------" << endl;
	for (auto iter = m_position_field_map.begin(); iter != m_position_field_map.end(); iter++)
	{
		if (iter->second->LongPosition + iter->second->ShortPosition > 0)
		{
			cerr << "合约代码：" << iter->second->instId << " ";
			if (iter->second->LongPosition > 0)
				cerr << "多单持仓量：" << iter->second->LongPosition << " "
				<< "多单今日持仓：" << iter->second->TodayLongPosition << " "
				<< "多单上日持仓：" << iter->second->YdLongPosition << " "
				<< "多单开仓均价：" << iter->second->LongAvgEntryPrice << " "
				<< "多单持仓均价：" << iter->second->LongAvgHoldingPrice << " "
				<< "多单持仓盈亏：" << iter->second->LongPositionProfit << " "
				<< "多单浮动盈亏：" << iter->second->LongOpenProfit << " "
				<< "多单平仓盈亏：" << iter->second->LongCloseProfit << " ";
			if (iter->second->ShortPosition > 0)
				cerr << "空单持仓量：" << iter->second->ShortPosition << " "
				<< "空单今日持仓：" << iter->second->TodayShortPosition << " "
				<< "空单上日持仓：" << iter->second->YdShortPosition << " "
				<< "空单开仓均价：" << iter->second->ShortAvgEntryPrice << " "
				<< "空单持仓均价：" << iter->second->ShortAvgHoldingPrice << " "
				<< "空单持仓盈亏：" << iter->second->ShortPositionProfit << " "
				<< "空单浮动盈亏：" << iter->second->ShortOpenProfit << " "
				<< "空单平仓盈亏：" << iter->second->ShortCloseProfit << " ";

				cerr<< "最新价：" << iter->second->LastPrice << " "
				<< "昨结价：" << iter->second->PreSettlementPrice << " "
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
	CThostFtdcOrderField* pOrder1 = NULL;//创建一个报单结构体指针
	std::lock_guard<std::mutex> m_lock(m_mutex);//加锁
	auto it = m_Orders.begin();
	for (; it != m_Orders.end(); it++)
	{
		if (pOrder->FrontID == it->second->FrontID && pOrder->SessionID == it->second->SessionID && strcmp(pOrder->OrderRef, it->second->OrderRef) == 0)
			break;
	}
	//找到了
	if (it != m_Orders.end()) {
		pOrder1 = it->second;
	}

	return pOrder1;
}
CThostFtdcOrderField* TdSpi::GetOrder(int nBrokerOrderSeq)
{
	CThostFtdcOrderField* pOrder = NULL;//创建一个报单结构体指针
	std::lock_guard<std::mutex> m_lock(m_mutex);//加锁
	map<int, CThostFtdcOrderField*>::iterator it = m_Orders.find(nBrokerOrderSeq);//
	//找到了
	if (it != m_Orders.end()) {
		pOrder = it->second;
	}

	return pOrder;
}

bool TdSpi::UpdateOrder(CThostFtdcOrderField* pOrder)
{
	//经纪公司的下单序列号,大于0表示已经接受报单
	if (pOrder->BrokerOrderSeq > 0) 
	{
		std::lock_guard<std::mutex> m_lock(m_mutex);//加锁，保证这个映射数据的安全
		//迭代器，查找是否有这个报单
		map<int, CThostFtdcOrderField*>::iterator it = m_Orders.find(pOrder->BrokerOrderSeq);
		//如果存在，我们需要更新它的状态
		if (it != m_Orders.end()) 
			//if (it != orderMap.end() && strcmp((it->second)->BrokerID, pOrder->BrokerID) == 0)
		{
			CThostFtdcOrderField* pOld = it->second;//把结构体的指针赋值给pOld
			//原报单已经关闭；
			char cOldStatus = pOld->OrderStatus;
			switch (cOldStatus) 
			{
			case THOST_FTDC_OST_AllTraded://全部成交
			case THOST_FTDC_OST_Canceled://已撤单
			case THOST_FTDC_OST_Touched://已经触发
				return false;
			case '6'://canceling//自己定义的，本程序已经发送了撤单请求，还在途中
				if (pOrder->OrderStatus != '0' && pOrder->OrderStatus != '5')
					return false;
			}
			memcpy(pOld, pOrder, sizeof(CThostFtdcOrderField));//更新报单的状态
			cerr << "TdSpi::UpdateOrder pOrder->OrderStatus :" << (it->second)->OrderStatus << endl;
			
			
		}
		//如果不存在，我们需要插入这个报单信息
		else 
		{
			CThostFtdcOrderField* pNew = new CThostFtdcOrderField();
			memcpy(pNew, pOrder, sizeof(CThostFtdcOrderField));
			m_Orders.insert(pair<int, CThostFtdcOrderField*>(pNew->BrokerOrderSeq, pNew));
		}
		return true;
	}
	//否则的话就不用加入到映射
	return false;
}

bool TdSpi::UpdateOrder(CThostFtdcOrderField* pOrder, std::map<int,CThostFtdcOrderField*>& orderMap)
{
	//经纪公司的下单序列号,大于0表示已经接受报单
	if (pOrder->BrokerOrderSeq > 0)
	{
		std::lock_guard<std::mutex> m_lock(m_mutex);//加锁，保证这个映射数据的安全
		//迭代器，查找是否有这个报单
		map<int, CThostFtdcOrderField*>::iterator it = orderMap.find(pOrder->BrokerOrderSeq);
		//如果存在，我们需要更新它的状态
		if (it != orderMap.end())
			//if (it != orderMap.end() && strcmp((it->second)->BrokerID, pOrder->BrokerID) == 0)
		{
			CThostFtdcOrderField* pOld = it->second;//把结构体的指针赋值给pOld
			//原报单已经关闭；
			char cOldStatus = pOld->OrderStatus;
			switch (cOldStatus)
			{
			case THOST_FTDC_OST_AllTraded://全部成交0
			case THOST_FTDC_OST_Canceled://已撤单5
			case THOST_FTDC_OST_Touched://已经触发c
				return false;
			case '6'://canceling//自己定义的，本程序已经发送了撤单请求，还在途中
				if (pOrder->OrderStatus != '0' && pOrder->OrderStatus != '5')
					return false;
			}
			memcpy(pOld, pOrder, sizeof(CThostFtdcOrderField));//更新报单的状态
			cerr << "TdSpi::UpdateOrder pOrder->OrderStatus :" << (it->second)->OrderStatus << endl;


		}
		//如果不存在，我们需要插入这个报单信息
		else
		{
			CThostFtdcOrderField* pNew = new CThostFtdcOrderField();
			memcpy(pNew, pOrder, sizeof(CThostFtdcOrderField));
			orderMap.insert(pair<int, CThostFtdcOrderField*>(pNew->BrokerOrderSeq, pNew));
		}
		return true;
	}
	//否则的话就不用加入到映射
	return false;
}


/// <summary>
/// 判断该持仓明细是否成交时间更早
/// </summary>
/// <param name="_tf">需要插入的持仓明细</param>
/// <param name="_curr">已经在映射中的某笔持仓明细</param>
/// <returns>是否更早</returns>
bool TdSpi::CompareRule(const CThostFtdcTradeField* _tf, const CThostFtdcTradeField* _curr)
{
			
	//首先判断日期
		if (std::strcmp(_tf->TradeDate, _curr->TradeDate) < 0)
			return true;

		else if (std::strcmp(_tf->TradeDate, _curr->TradeDate) == 0)
		{
			//其次判断交易所代码
			if (std::strcmp(_tf->ExchangeID, _curr->ExchangeID) < 0)
				return true;
			else if (std::strcmp(_tf->ExchangeID, _curr->ExchangeID) == 0)
			{
				//第三判断 TradeID
				if (std::strcmp(_tf->TradeID, _curr->TradeID) < 0)
					return true;

			}

		}
		return false;
}


int TdSpi::GetNextRequestID()
{
	//给m_nNextRequestID加上互斥锁
	/*m_mutex.lock();
	int nNextID = m_nNextRequestID++;
	m_mutex.unlock();*/
	//1原理，在构造函数里面使用m_mutex.lock();
	//在析构的时候使用解锁m_mutex.unlock();
std:lock_guard<mutex> m_lock(m_mutex);
	
	int nNextID = g_nRequestID++;

	return g_nRequestID;
}
char * TdSpi::GetNextOrderRef()
{
	//加锁
	
std:lock_guard<mutex> m_lock(m_mutex);

	int nNextOrderRef =atoi(orderRef);

	sprintf_s(orderRef, sizeof(orderRef), "%012d", ++nNextOrderRef);

	return orderRef;
}
//
//bool TdSpi::compareRule(CThostFtdcTradeField* _left, CThostFtdcTradeField* _right)
//{
//	//首先判断日期
//	if (strcmp(_left->TradeDate, _right->TradeDate) < 0)
//		return true;
//
//	else if (strcmp(_left->TradeDate, _right->TradeDate) == 0)
//	{
//		//其次判断交易所代码
//		if (strcmp(_left->ExchangeID, _right->ExchangeID) < 0)
//			return true;
//		else if (strcmp(_left->ExchangeID, _right->ExchangeID) == 0)
//		{
//			//第三判断 TradeID
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
		
		cerr << "响应 | 合约：" << pInstrument->InstrumentID
			<< "合约名称：" << pInstrument->InstrumentName
			<< " 合约在交易所代码：" << pInstrument->ExchangeInstID
			<< " 产品代码：" << pInstrument->ProductID
			<< " 产品类型：" << pInstrument->ProductClass
			<< " 多头保证金率：" << pInstrument->LongMarginRatio
			<< " 空头保证金率：" << pInstrument->ShortMarginRatio
			<< " 合约数量乘数：" << pInstrument->VolumeMultiple
			<< " 最小变动价位：" << pInstrument->PriceTick
			<< " 交易所代码：" << pInstrument->ExchangeID
			<< " 交割年份：" << pInstrument->DeliveryYear
			<< " 交割月：" << pInstrument->DeliveryMonth
			<< " 创建日：" << pInstrument->CreateDate
			<< " 到期日：" << pInstrument->ExpireDate
			<< " 上市日：" << pInstrument->OpenDate
			<< " 开始交割日：" << pInstrument->StartDelivDate
			<< " 结束交割日：" << pInstrument->EndDelivDate
			<< " 合约生命周期状态：" << pInstrument->InstLifePhase
			<< " 当前是否交易：" << pInstrument->IsTrading << endl;
	}
}

bool TdSpi::FindTrade(CThostFtdcTradeField* pTrade)
{
	//ExchangeID //交易所代码
	//	TradeID   //成交编号
	//	Direction  //买卖方向
	std::lock_guard<std::mutex> m_lock(m_mutex);//加锁，保证这个set数据的安全
	for (auto it = tradeList.begin(); it != tradeList.end(); it++)
	{
		//判断是否已经存在
		if (strcmp((*it)->ExchangeID, pTrade->ExchangeID) ==0&& 
			strcmp((*it)->TradeID, pTrade->TradeID) == 0&& (*it)->Direction== pTrade->Direction)
			return true;
	}

	return false;
}

bool TdSpi::FindTrade(CThostFtdcTradeField* pTrade, std::vector<CThostFtdcTradeField*>& tdList)
{
	//ExchangeID //交易所代码
	//	TradeID   //成交编号
	//	Direction  //买卖方向
	std::lock_guard<std::mutex> m_lock(m_mutex);//加锁，保证这个set数据的安全
	for (auto it = tdList.begin(); it != tdList.end(); it++)
	{
		//判断是否已经存在
		if (strcmp((*it)->ExchangeID, pTrade->ExchangeID) == 0 &&
			strcmp((*it)->TradeID, pTrade->TradeID) == 0 && (*it)->Direction == pTrade->Direction)
			return true;
	}

	return false;
}

void TdSpi::InsertTrade(CThostFtdcTradeField* pTrade)
{
	std::lock_guard<std::mutex> m_lock(m_mutex);//加锁，保证这个set数据的安全
	CThostFtdcTradeField* trade = new CThostFtdcTradeField();//创建trade，分配堆空间，记得在析构函数里面要delete
	memcpy(trade, pTrade, sizeof(CThostFtdcTradeField));//pTrade复制给trade
	tradeList.push_back(trade);//输入录入
}

void TdSpi::InsertTrade(CThostFtdcTradeField* pTrade, std::vector<CThostFtdcTradeField*>& tdList)
{
	std::lock_guard<std::mutex> m_lock(m_mutex);//加锁，保证这个set数据的安全
	CThostFtdcTradeField* trade = new CThostFtdcTradeField();//创建trade，分配堆空间，记得在析构函数里面要delete
	memcpy(trade, pTrade, sizeof(CThostFtdcTradeField));//pTrade复制给trade
	tdList.push_back(trade);
}

void TdSpi::ShowTradeList()
{
	std::lock_guard<std::mutex> m_lock(m_mutex);//加锁，保证这个set数据的安全
	cerr << endl << endl;
	cerr << "---------------打印成交明细-------------" << endl;
	for (auto iter = tradeList.begin(); iter != tradeList.end(); iter++)
	{
		cerr << endl << "投资者代码：" << (*iter)->InvestorID << "  "
			<< "用户代码：" << (*iter)->UserID << "  " << "成交编号：" << (*iter)->TradeID << "  "
			<< "合约代码：" << (*iter)->InstrumentID << "  " << "买卖方向：" << (*iter)->Direction << "  "
			<< "开平：" << (*iter)->OffsetFlag << "  " << "投机/套保" << (*iter)->HedgeFlag << "  "
			<< "价格：" << (*iter)->Price << "  " << "数量：" << (*iter)->Volume << "  "
			<< "报单引用：" << (*iter)->OrderRef << "  " << "本地报单编号：" << (*iter)->OrderLocalID << "  "
			<< "成交时间：" << (*iter)->TradeTime << "  " << "业务单元：" << (*iter)->BusinessUnit << "  "
			<< "序号：" << (*iter)->SequenceNo << "  " << "经纪公司下单序号：" << (*iter)->BrokerOrderSeq << "  "
			<< "交易日：" << (*iter)->TradingDay << endl;
	}

	cerr << endl << endl;
}

void TdSpi::ShowTradeList(std::vector<CThostFtdcTradeField*>& tdList)
{
	std::lock_guard<std::mutex> m_lock(m_mutex);//加锁，保证这个set数据的安全
	cerr << endl << endl;
	cerr << "---------------打印成交明细-------------" << endl;
	for (auto iter = tdList.begin(); iter != tdList.end(); iter++)
	{
		cerr << endl << "投资者代码：" << (*iter)->InvestorID << "  "
			<< "用户代码：" << (*iter)->UserID << "  " << "成交编号：" << (*iter)->TradeID << "  "
			<< "合约代码：" << (*iter)->InstrumentID << "  " << "买卖方向：" << (*iter)->Direction << "  "
			<< "开平：" << (*iter)->OffsetFlag << "  " << "投机/套保" << (*iter)->HedgeFlag << "  "
			<< "价格：" << (*iter)->Price << "  " << "数量：" << (*iter)->Volume << "  "
			<< "报单引用：" << (*iter)->OrderRef << "  " << "本地报单编号：" << (*iter)->OrderLocalID << "  "
			<< "成交时间：" << (*iter)->TradeTime << "  " << "业务单元：" << (*iter)->BusinessUnit << "  "
			<< "序号：" << (*iter)->SequenceNo << "  " << "经纪公司下单序号：" << (*iter)->BrokerOrderSeq << "  "
			<< "交易日：" << (*iter)->TradingDay << endl;
	}

	cerr << endl << endl;
}

void TdSpi::ShowOrderList(std::map<int, CThostFtdcOrderField*>& orderList)
{
	if (orderList.size() > 0)
	{
		cerr << endl<<endl<<endl;
		cerr << "总的报单次数：" << orderList.size() << endl;
		cerr << "---------------打印报单开始---------------" << endl;
		for (auto iter = orderList.begin(); iter != orderList.end(); iter++)
		{
			cerr << "经纪公司代码：" << (iter->second)->BrokerID << "  " << "投资者代码：" << (iter->second)->InvestorID << "  "
				<< "用户代码：" << (iter->second)->UserID << "  " << "合约代码：" << (iter->second)->InstrumentID << "  "
				<< "买卖方向：" << (iter->second)->Direction << "  " << "组合开平标志：" << (iter->second)->CombOffsetFlag << "  "
				<< "价格：" << (iter->second)->LimitPrice << "  " << "数量：" << (iter->second)->VolumeTotalOriginal << "  "
				<< "报单引用：" << (iter->second)->OrderRef << "  " << "客户代码：" << (iter->second)->ClientID << "  "
				<< "报单状态：" << (iter->second)->OrderStatus << "  " << "委托时间：" << (iter->second)->InsertTime << "  "
				<< "报单编号：" << (iter->second)->OrderStatus << "  " << "交易日：" << (iter->second)->TradingDay << "  "
				<< "报单日期：" << (iter->second)->InsertDate << endl;

		}
		cerr << "---------------打印报单完成---------------" << endl;
		cerr << endl << endl << endl;
	}
}

//更新持仓明细
bool TdSpi::UpdatePositionDetail(CThostFtdcTradeField* pTrade)
{
	//数据为空，不更新返回false
	if (!pTrade)
		return false;

	std::lock_guard<std::mutex> m_lock(m_mutex);//加锁，保证这个数据的安全
	
	
												
	//1、如果是开仓，则插入trade为新的持仓明细
	if (pTrade->TradeID && pTrade->OffsetFlag == THOST_FTDC_OF_Open)
	{
		CThostFtdcTradeField* trade = new CThostFtdcTradeField();//创建CThostFtdcTradeField *，申请了内存空间
		memcpy(trade, pTrade, sizeof(CThostFtdcTradeField));
		//在持仓明细结构体中，我们用trade->TradeDate来保存该笔成交所属的交易日,等同于openDate（开仓的交易日）
		strcpy(trade->TradeDate, m_cTradingDay);//上海期货交易，tradingday有也是正确的
		strcpy(trade->TradingDay, m_cTradingDay);//对应模拟环境，trade->TradingDay信息有些交易所是空的trade->TradingDay==""
		if (pTrade->Direction == '0')//买入方向
			positionDetailList_NotClosed_Long.push_back(trade);
		else if (pTrade->Direction == '1')//卖出方向
			positionDetailList_NotClosed_Short.push_back(trade);
	}
	else//如果是平仓则删除对应的持仓明细
	{
		int vol = pTrade->Volume;//成交手数
		
		std::list<CThostFtdcTradeField*>* pos = nullptr;

		if (pTrade->Direction == '0')//买入方向，买入平仓，空单的持仓明细
		{
			pos = &positionDetailList_NotClosed_Short;
		}
		else if (pTrade->Direction == '1')//卖出方向，卖出平仓，多单的持仓明细
		{
			pos = &positionDetailList_NotClosed_Long;
		}
		//如果为空，没有持仓明细数据可以供我们删除
		if (!pos)
			return false;


		int   j = 0;//用来加总符合条件的仓位手数
		bool findflag = false;//标志，是否找到第一笔符合条件的持仓明细
		
							  //是否是平今
		bool closeTd = pTrade->OffsetFlag == THOST_FTDC_OF_CloseToday && (strcmp(pTrade->ExchangeID, "SHFE") == 0 || strcmp(pTrade->ExchangeID, "INE") == 0);
		
		//------------新增加的内容-------------，记录开仓均价
		double avgOpenPrice = 0.0;
		double avgHoldingPrice = 0.0;


		//计算哪些明细和平仓对应
		for (auto it = (*pos).begin(); it != (*pos).end(); it++)

		{
			if (strcmp((*it)->InstrumentID, pTrade->InstrumentID) == 0)//是否是该平仓合约
			{
				if (closeTd)//平今，则需要查找持仓明细中第一笔当日开仓的持仓
				{
					
					bool isToday = strcmp((*it)->TradeDate, m_cTradingDay) == 0 ;
					
					//修改
					if (!findflag && isToday)
						findflag = true;
					if (findflag && isToday)
					{
						int temp = j;
						//判断第i个持仓明细的手数是否足够vol-temp
						j = temp + ((*it)->Volume > (vol - temp) ? (vol - temp) : (*it)->Volume);
						//比如我的平仓量是5手，temp为0，是否是大于vol - temp，temp+这个数
						(*it)->Volume -= (j - temp);//持仓量减少（j - temp）

						//------------------新增加内容---------,加总开仓金额
						avgOpenPrice += (double)(j - temp) * (*it)->Price;

					
					}
				}
				else//平仓，
				{
					//上期所或者能源所，平昨
					if (strcmp(pTrade->ExchangeID, "SHFE") == 0 || strcmp(pTrade->ExchangeID, "INE") == 0)
					{
					//
						if (!findflag && strcmp((*it)->TradeDate, m_cTradingDay) != 0)
							findflag = true;
						if (findflag && strcmp((*it)->TradeDate, m_cTradingDay) != 0)
						{
							int temp = j;
							//判断第i个持仓明细的手数是否足够vol-temp
							j = temp + ((*it)->Volume > (vol - temp) ? (vol - temp) : (*it)->Volume);
							(*it)->Volume -= (j - temp);
							//------------------新增加内容---------,加总开仓金额
							avgOpenPrice += (double)(j - temp) * (*it)->Price;
							

						}
					}
					else//中金所、大商所、郑商所平仓
					{
						if (!findflag)
							findflag = true;
						if (findflag)
						{
							int temp = j;
							j = temp + ((*it)->Volume > (vol - temp) ? (vol - temp) : (*it)->Volume);
							(*it)->Volume -= (j - temp);

							//------------------新增加内容---------,加总开仓金额
							avgOpenPrice += (double)(j - temp) * (*it)->Price;
						}
					}
				}
				if (j == vol)
					break;
			}
		}
		
		//删除持仓量为0的持仓明细
		for (auto it = (*pos).begin(); it != (*pos).end();)
		{
			if ((*it)->Volume == 0)
				it = (*pos).erase(it);//erase返回it后面的一个迭代器，不能再it++了
			else
				it++;
		}

		//------------------新增加内容---------,计算开仓均价、平仓盈亏
		string strInst = pTrade->InstrumentID;
		auto it = m_position_field_map.find(strInst);
		if (it != m_position_field_map.end())
		{
			//计算开仓均价
			avgOpenPrice /= vol;
			
			int multi = 0;
			auto instIt = m_all_instruments_field_map.find(strInst);
			//如果找到该合约
			if (instIt != m_all_instruments_field_map.end())
			{
				multi = instIt->second->VolumeMultiple;
				//计算平仓盈亏
				if (pTrade->Direction == '0')//买入方向,买入平仓，对应我们的空单
				{
					it->second->ShortCloseProfit += (avgOpenPrice - pTrade->Price) * multi * vol;
					

				}
				else//卖出平仓，对应我们的多单
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
	std::lock_guard<std::mutex> m_lock(m_mutex);//加锁，保证这个set数据的安全
	cerr << endl << endl;
	cerr << "---------------打印多单持仓明细-------------" << endl;
	for (auto iter = positionDetailList_NotClosed_Long.begin(); iter != positionDetailList_NotClosed_Long.end(); iter++)
	{
		cerr << endl << "投资者代码：" << (*iter)->InvestorID << "  "
			<< "用户代码：" << (*iter)->UserID << "  " << "成交编号：" << (*iter)->TradeID << "  "
			<< "合约代码：" << (*iter)->InstrumentID << "  " << "买卖方向：" << (*iter)->Direction << "  "
			<< "开平：" << (*iter)->OffsetFlag << "  " << "投机/套保" << (*iter)->HedgeFlag << "  "
			<< "价格：" << (*iter)->Price << "  " << "数量：" << (*iter)->Volume << "  "
			<< "报单引用：" << (*iter)->OrderRef << "  " << "本地报单编号：" << (*iter)->OrderLocalID << "  "
			<< "成交时间：" << (*iter)->TradeTime << "  " << "业务单元：" << (*iter)->BusinessUnit << "  "
			<< "序号：" << (*iter)->SequenceNo << "  " << "经纪公司下单序号：" << (*iter)->BrokerOrderSeq << "  "
			<< "交易日：" << (*iter)->TradingDay << endl;
	}

	cerr << endl << endl;

	cerr << "---------------打印空单持仓明细-------------" << endl;
	for (auto iter = positionDetailList_NotClosed_Short.begin(); iter != positionDetailList_NotClosed_Short.end(); iter++)
	{
		cerr << endl << "投资者代码：" << (*iter)->InvestorID << "  "
			<< "用户代码：" << (*iter)->UserID << "  " << "成交编号：" << (*iter)->TradeID << "  "
			<< "合约代码：" << (*iter)->InstrumentID << "  " << "买卖方向：" << (*iter)->Direction << "  "
			<< "开平：" << (*iter)->OffsetFlag << "  " << "投机/套保" << (*iter)->HedgeFlag << "  "
			<< "价格：" << (*iter)->Price << "  " << "数量：" << (*iter)->Volume << "  "
			<< "报单引用：" << (*iter)->OrderRef << "  " << "本地报单编号：" << (*iter)->OrderLocalID << "  "
			<< "成交时间：" << (*iter)->TradeTime << "  " << "业务单元：" << (*iter)->BusinessUnit << "  "
			<< "序号：" << (*iter)->SequenceNo << "  " << "经纪公司下单序号：" << (*iter)->BrokerOrderSeq << "  "
			<< "交易日：" << (*iter)->TradingDay << endl;
	}

	cerr << endl << endl;
}

void TdSpi::UpdatePositionFieldMap(TThostFtdcInstrumentIDType InstrumentID)
{
	// 查找合约是否存在
	bool bFind = FindPositionInst(InstrumentID);
	if (!bFind)
	{
		InsertPositionInst(InstrumentID);
	}

}
/// <summary>
/// 在持仓合约映射中查找指定的合约
/// </summary>
/// <param name="InstrumentID"> 指定的合约名称</param>
/// <returns>返回是否找到</returns>
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
	cerr << "-----------------------没有这个合约，需要构造交易信息结构体" << endl;
	position_field* p_position_field = new position_field();
	p_position_field->instId = InstrumentID;
	//构造持仓合约的string
	m_Inst_Position += InstrumentID;
	m_Inst_Position += ",";
	m_position_field_map.insert(pair<string, position_field*>(InstrumentID, p_position_field));
}
/// <summary>
/// 在订阅合约的subscribe_inst_vec容器中查找该合约是否存在
/// </summary>
/// <param name="InstrumentID">指定合约名称</param>
/// <returns>合约是否包含在容器中</returns>
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
	//浮动盈亏
	double openProfit = 0.0;
	//持仓盈亏
	double positionProfit = 0.0;
	//平仓盈亏
	double closeProfit = 0.0;
	//加锁
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
	cerr << "账户浮动盈亏 " << m_OpenProfit << "账户持仓盈亏 " << m_PositionProfit << "账户平仓盈亏 " << m_CloseProfit << endl;
	

}

void TdSpi::ReFreshPositionField(std::string strInst, position_field* pf)
{
	position_field pfo;
	//是否能够在持仓明细中找到该合约
	bool  findInst = false;
	//1、在多单的持仓明细中查找，更新多头持仓数量和平均价
	for (auto it = positionDetailList_NotClosed_Long.begin(); it != positionDetailList_NotClosed_Long.end(); it++)
	{
		//判断合约是否是指定合约
		if (strcmp(strInst.c_str(), (*it)->InstrumentID) == 0)
		{
			findInst = true;
			//1）加总持仓手数
			pfo.LongPosition += (*it)->Volume;
			//2)加总今仓手数
			if (strcmp(m_cTradingDay, (*it)->TradeDate) == 0)//有个坑，有些交易所只提供了TradeDate
				//TradeDate,我们用TradeDate保存了该笔交易开仓时的（交易日），TradingDay今天的交易日
				//今天是1月4日，现在是夜盘交易时间，那么TradeDate就是1月5日，TradingDay为1月5日
			//if (strcmp(m_cTradingDay, (*it)->TradeDate) == 0 || strcmp(m_cTradingDay, (*it)->TradingDay) == 0)//有个坑，有些交易所只提供了TradeDate
			{
				pfo.TodayLongPosition+= (*it)->Volume;
				//5)加总持仓金额，对应今仓使用开仓价，后面用来计算持仓均价
				pfo.LongAvgHoldingPrice+= (*it)->Price* (*it)->Volume;
			}
			else//3)加总昨仓手数
			{
				pfo.YdLongPosition += (*it)->Volume;
				//5)加总持仓金额，对应老仓使用昨结价，用来计算持仓均价
				pfo.LongAvgHoldingPrice += pf->PreSettlementPrice * (*it)->Volume;
			}

			//4)加总开仓金额，用来计算开仓均价
			pfo.LongAvgEntryPrice += (*it)->Price * (*it)->Volume;
			
		}
	}
	//如果仓位不为0，可以计算两个均价
	if (pfo.LongPosition > 0)
	{
		pfo.LongAvgEntryPrice /= pfo.LongPosition;
		pfo.LongAvgHoldingPrice /= pfo.LongPosition;

		//6)计算持仓盈亏和浮动（开仓）盈亏
		//首先需要合约乘数
		int multi = 0;
		auto instIt = m_all_instruments_field_map.find(strInst);
		//如果找到该合约
		if (instIt != m_all_instruments_field_map.end())
		{
			multi = instIt->second->VolumeMultiple;
			//计算浮动（开仓）盈亏
			pfo.LongOpenProfit = (pf->LastPrice - pfo.LongAvgEntryPrice) * multi * pfo.LongPosition;
			//计算持仓盈亏
			pfo.LongPositionProfit = (pf->LastPrice - pfo.LongAvgHoldingPrice) * multi * pfo.LongPosition;
		}
		//计算结果赋值给pf指向的对象
		//三个仓位
		pf->LongPosition = pfo.LongPosition;
		pf->TodayLongPosition = pfo.TodayLongPosition;
		pf->YdLongPosition = pfo.YdLongPosition;
		//两个价格
		pf->LongAvgEntryPrice = pfo.LongAvgEntryPrice;
		pf->LongAvgHoldingPrice = pfo.LongAvgHoldingPrice;

		//两个盈亏
		pf->LongPositionProfit = pfo.LongPositionProfit;
		pf->LongOpenProfit = pfo.LongOpenProfit;

	}
	//pfo.LongAvgHoldingProfit
	if (!findInst)//多头仓位中没有该合约，需要把仓位、今仓、昨仓及开仓价、持仓价均设置为0
	{
		//三个仓位
		pf->LongPosition = 0;
		pf->TodayLongPosition = 0;
		pf->YdLongPosition = 0;
		//两个价格
		pf->LongAvgEntryPrice = 0.0;
		pf->LongAvgHoldingPrice = 0.0;

		//两个盈亏
		pf->LongPositionProfit = 0.0;
		pf->LongOpenProfit = 0.0;
	}

	findInst = false;




	//1、在空单的持仓明细中查找，更新空头持仓数量和平均价
	for (auto it = positionDetailList_NotClosed_Short.begin(); it != positionDetailList_NotClosed_Short.end(); it++)
	{
		//判断合约是否是指定合约
		if (strcmp(strInst.c_str(), (*it)->InstrumentID) == 0)
		{
			findInst = true;
			//1）加总持仓手数
			pfo.ShortPosition += (*it)->Volume;
			//2)加总今仓手数
			if (strcmp(m_cTradingDay, (*it)->TradeDate) == 0)//有个坑，有些交易所只提供了TradeDate
				//if (strcmp(m_cTradingDay, (*it)->TradeDate) == 0 || strcmp(m_cTradingDay, (*it)->TradingDay) == 0)//有个坑，有些交易所只提供了TradeDate
			{
				pfo.TodayShortPosition += (*it)->Volume;
				//5)加总持仓金额，对应今仓使用开仓价，后面用来计算持仓均价
				pfo.ShortAvgHoldingPrice += (*it)->Price * (*it)->Volume;
			}
			else//3)加总昨仓手数
			{
				pfo.YdShortPosition += (*it)->Volume;
				//5)加总持仓金额，对应老仓使用昨结价，用来计算持仓均价
				pfo.ShortAvgHoldingPrice += pf->PreSettlementPrice * (*it)->Volume;
			}

			//4)加总开仓金额，用来计算开仓均价
			pfo.ShortAvgEntryPrice += (*it)->Price * (*it)->Volume;

		}
	}
	//如果仓位不为0，可以计算两个均价
	if (pfo.ShortPosition > 0)
	{
		pfo.ShortAvgEntryPrice /= pfo.ShortPosition;
		pfo.ShortAvgHoldingPrice /= pfo.ShortPosition;

		//6)计算持仓盈亏和浮动（开仓）盈亏
		//首先需要合约乘数
		int multi = 0;
		auto instIt = m_all_instruments_field_map.find(strInst);
		//如果找到该合约
		if (instIt != m_all_instruments_field_map.end())
		{
			multi = instIt->second->VolumeMultiple;
			//计算浮动（开仓）盈亏
			pfo.ShortOpenProfit = (pfo.ShortAvgEntryPrice - pf->LastPrice) * multi * pfo.ShortPosition;
			//计算持仓盈亏
			pfo.ShortPositionProfit = (pfo.ShortAvgHoldingPrice - pf->LastPrice ) * multi * pfo.ShortPosition;
		}
		//计算结果赋值给pf指向的对象
		//三个仓位
		pf->ShortPosition = pfo.ShortPosition;
		pf->TodayShortPosition = pfo.TodayShortPosition;
		pf->YdShortPosition = pfo.YdShortPosition;
		//两个价格
		pf->ShortAvgEntryPrice = pfo.ShortAvgEntryPrice;
		pf->ShortAvgHoldingPrice = pfo.ShortAvgHoldingPrice;

		//两个盈亏
		pf->ShortPositionProfit = pfo.ShortPositionProfit;
		pf->ShortOpenProfit = pfo.ShortOpenProfit;

	}
	//pfo.LongAvgHoldingProfit
	if (!findInst)//空头仓位中没有该合约，需要把仓位、今仓、昨仓及开仓价、持仓价均设置为0
	{
		//三个仓位
		pf->ShortPosition = 0;
		pf->TodayShortPosition = 0;
		pf->YdShortPosition = 0;
		//两个价格
		pf->ShortAvgEntryPrice = 0.0;
		pf->ShortAvgHoldingPrice = 0.0;

		//两个盈亏
		pf->ShortPositionProfit = 0.0;
		pf->ShortOpenProfit = 0.0;
	}





}

bool TdSpi::BeUpDatedMDPrice()
{
	//加锁
	std::lock_guard<std::mutex> m_lock(m_mutex);
	auto it = m_position_field_map.begin();
	while (it != m_position_field_map.end())
	{
		if (it->second->LastPrice < 0.0001 || it->second->PreSettlementPrice < 0.0001)
			break;
		it++;
	}
	//如果条件成立，表明最新价和昨结价都有了
	bUpdateMdPrice = (it == m_position_field_map.end());
	return bUpdateMdPrice;
}

void TdSpi::UpdatePositionMDPrice(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	
	//数据过滤，过滤一些异常数据
	//if(pDepthMarketData->UpdateTime)
	if (pDepthMarketData->PreSettlementPrice < 0.001 || pDepthMarketData->LastPrice < 0.001)
		return;
	if (pDepthMarketData->LastPrice < pDepthMarketData->LowerLimitPrice || pDepthMarketData->LastPrice > pDepthMarketData->UpperLimitPrice)
		return;


	string strInst = pDepthMarketData->InstrumentID;
	//加锁
	std::lock_guard<std::mutex> m_lock(m_mutex);
	//在映射内寻找该合约
	auto it = m_position_field_map.find(strInst);
	//找到了该合约
	if (it != m_position_field_map.end())
	{
		if(it->second->PreSettlementPrice < 0.1)
		it->second->PreSettlementPrice = pDepthMarketData->PreSettlementPrice;
		it->second->LastPrice = pDepthMarketData->LastPrice;
	}

}

void TdSpi::Release()
{
	//清空orderList，当天的所有报单数据
	for (auto it = orderList.begin(); it != orderList.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	orderList.clear();
	

	//清空m_mOrders，当天的所有报单数据
	for (auto it = m_mOrders.begin(); it != m_mOrders.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	m_mOrders.clear();
	//当天的所有成交
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
	////用于保存所有期货合约的映射
	//std::map<std::string, CThostFtdcInstrumentField*> m_all_instruments_field_map;

	for (auto it = m_all_instruments_field_map.begin(); it != m_all_instruments_field_map.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	m_all_instruments_field_map.clear();
	
	////本程序的报单集合
	//std::map<int, CThostFtdcOrderField*> m_Orders;
	
	for (auto it = m_Orders.begin(); it != m_Orders.end(); it++)
	{
		delete it->second;
		it->second = nullptr;
	}
	m_Orders.clear();

	////本程序的成交vector
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
	//创建报单结构体对象
	CThostFtdcInputOrderField orderField;
	//初始化清零
	memset(&orderField, 0, sizeof(CThostFtdcInputOrderField));

	//fill the broker and user fields;
	
	strcpy(orderField.BrokerID, m_BrokerId.c_str());//复制我们的brokerid到结构体对象
	strcpy(orderField.InvestorID, m_UserId.c_str());//复制我们的InvestorID到结构体对象

	//set the Symbol code;
	strcpy(orderField.InstrumentID, pszCode);//设置下单的期货合约
	//CThostFtdcInstrumentField* instField = m_all_instruments_field_map.find(pszCode)->second;
	/*if (instField)
	{
		const char* ExID = instField->ExchangeID;
		strcpy(orderField.ExchangeID, ExID);
	}*/
	strcpy(orderField.ExchangeID, pszExchangeID);//交易所代码，"SHFE","CFFEX","DCE","CZCE","INE"
	if (nDirection == 0) {
		orderField.Direction = THOST_FTDC_D_Buy;
	}
	else {
		orderField.Direction = THOST_FTDC_D_Sell;
	}

	orderField.LimitPrice = fPrice;//价格

	orderField.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;//投机还是套利，套保，做市等

	
	
	orderField.VolumeTotalOriginal = nVolume;//下单手数

	//--------------1、价格条件----------------
		//限价单；
	orderField.OrderPriceType = THOST_FTDC_OPT_LimitPrice;//报单的价格类型条件

	//市价单；
	//orderField.OrderPriceType=THOST_FTDC_OPT_AnyPrice;
	//中金所市价
	//orderField.OrderPriceType = THOST_FTDC_OPT_FiveLevelPrice;

	//--------------2、触发条件----------------
	//	///立即
//#define THOST_FTDC_CC_Immediately '1'
/////止损
//#define THOST_FTDC_CC_Touch '2'
/////止赢
//#define THOST_FTDC_CC_TouchProfit '3'
/////预埋单
//#define THOST_FTDC_CC_ParkedOrder '4'
/////最新价大于条件价
//#define THOST_FTDC_CC_LastPriceGreaterThanStopPrice '5'
/////最新价大于等于条件价
//#define THOST_FTDC_CC_LastPriceGreaterEqualStopPrice '6'
/////最新价小于条件价
//#define THOST_FTDC_CC_LastPriceLesserThanStopPrice '7'
/////最新价小于等于条件价
//#define THOST_FTDC_CC_LastPriceLesserEqualStopPrice '8'
/////卖一价大于条件价
//#define THOST_FTDC_CC_AskPriceGreaterThanStopPrice '9'
/////卖一价大于等于条件价
//#define THOST_FTDC_CC_AskPriceGreaterEqualStopPrice 'A'
/////卖一价小于条件价
//#define THOST_FTDC_CC_AskPriceLesserThanStopPrice 'B'
/////卖一价小于等于条件价
//#define THOST_FTDC_CC_AskPriceLesserEqualStopPrice 'C'
/////买一价大于条件价
//#define THOST_FTDC_CC_BidPriceGreaterThanStopPrice 'D'
/////买一价大于等于条件价
//#define THOST_FTDC_CC_BidPriceGreaterEqualStopPrice 'E'
/////买一价小于条件价
//#define THOST_FTDC_CC_BidPriceLesserThanStopPrice 'F'
/////买一价小于等于条件价
//#define THOST_FTDC_CC_BidPriceLesserEqualStopPrice 'H'

	orderField.ContingentCondition = THOST_FTDC_CC_Immediately;//报单的触发条件
	//orderField.ContingentCondition = THOST_FTDC_CC_LastPriceGreaterThanStopPrice;//报单的触发条件
	//orderField.StopPrice = 5035.0;//



	//--------------3、时间条件----------------
	//orderField.TimeCondition = THOST_FTDC_TC_IOC;

//	///立即完成，否则撤销
//#define THOST_FTDC_TC_IOC '1'
/////本节有效
//#define THOST_FTDC_TC_GFS '2'
/////当日有效
//#define THOST_FTDC_TC_GFD '3'
/////指定日期前有效
//#define THOST_FTDC_TC_GTD '4'
/////撤销前有效
//#define THOST_FTDC_TC_GTC '5'
/////集合竞价有效
//#define THOST_FTDC_TC_GFA '6'


	//orderField.TimeCondition = THOST_FTDC_TC_GFS;//时间条件,本节有效,------没法用，错单，上期所、中金所：不被支持的报单类型
	//orderField.TimeCondition = THOST_FTDC_TC_GTD;//时间条件,指定日期有效,错单，不被支持的报单类型
	//orderField.TimeCondition = THOST_FTDC_TC_GTC;//时间条件,撤销前有效,错单，不被支持的报单类型
	//orderField.TimeCondition = THOST_FTDC_TC_GFA;//时间条件,集合竞价有效,错单，不被支持的报单类型


	//orderField.TimeCondition = THOST_FTDC_TC_IOC;//时间条件，立即完成，否则撤销
	orderField.TimeCondition = THOST_FTDC_TC_GFD;//时间条件,当日有效


	//--------------4、数量条件----------------
	orderField.VolumeCondition = THOST_FTDC_VC_AV;//任意数量

	//orderField.VolumeCondition = THOST_FTDC_VC_MV;//最小数量
	//orderField.VolumeCondition = THOST_FTDC_VC_CV;//全部数量


	

	strcpy(orderField.GTDDate, m_cTradingDay);//下单的日期



	orderField.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;//强平原因

	switch (nOpenClose) {
	case 0:
		orderField.CombOffsetFlag[0] = THOST_FTDC_OF_Open;//开仓
		break;
	case 1:
		orderField.CombOffsetFlag[0] = THOST_FTDC_OF_Close;//平仓
		break;
	case 2:
		orderField.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;//平今仓
		break;
	case 3:
		orderField.CombOffsetFlag[0] = THOST_FTDC_OF_CloseYesterday;//平昨仓
		break;
	}

	
	//******************************报单引用测试***********************************
	
	/*sprintf_s(orderRef, sizeof(orderRef), "%012d", 1);
	orderField.OrderRef*/
	GetNextOrderRef();
	strcpy(orderField.OrderRef, orderRef);//报单引用

	//******************************报单引用测试***********************************




	//调用交易的api的ReqOrderInsert
	int retCode = m_pUserTDApi_trade->ReqOrderInsert(&orderField, GetNextRequestID());
	
	//改成了错误的
	//int retCode = m_pUserTDApi_trade->ReqOrderInsert(&orderField, 1);

	if (retCode != 0) {
		printf("failed to insert order,instrument: %s, volume: %d, price: %f\n", pszCode, nVolume, fPrice);
	}


	//新增加的内容
	//CThostFtdcOrderField结构体指针，新申请堆空间
	CThostFtdcOrderField* pOrder = new CThostFtdcOrderField;
	//初始化清零
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


	strcpy(pOrder->GTDDate, orderField.GTDDate);//下单的日期
	pOrder->ForceCloseReason = orderField.ForceCloseReason;//强平原因
	pOrder->CombOffsetFlag[0] = orderField.CombOffsetFlag[0];//开平
	pOrder->FrontID = m_nFrontID;
	pOrder->SessionID = m_nSessionID;
	strcpy(pOrder->OrderRef, orderField.OrderRef);
	char str[50] = { '\0' };
	sprintf_s(str, sizeof(str), "%d%d%s", pOrder->FrontID, pOrder->SessionID, pOrder->OrderRef);
	m_mOrders.insert(make_pair(str, pOrder));
	return pOrder;

}
