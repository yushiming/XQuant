#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"ThostFtdcTraderApi.h"
#include"MdSpi.h"//mdspi.h已经包含了下面的头文件
#include"mystruct.h"
#include<map>
#include<vector>
#include<set>
#include<list>

//#include"ThostFtdcMdApi.h"
class TdSpi :public CThostFtdcTraderSpi {
public:
	//构造函数
	TdSpi(CThostFtdcTraderApi* tdapi, CThostFtdcMdApi* pUserApi_md, MdSpi* pUserSpi_md);
	~TdSpi();
	//当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	void OnFrontConnected();
	
	//请求用户登录
	int ReqUserLogin();

	///客户端认证请求
	int ReqAuthenticate();

	//请求查询订单
	void ReqQryOrder();

	//请求查询成交
	void ReqQryTrade();

	//请求查询持仓明细
	void ReqQryInvestorPositionDetail();

	void ReqQryTradingAccount();
	void ReqQryInvestorPosition_All();
	void ReqQryInvestorPosition(char* pInstrument);
	void ReqQryInstrumetAll();
	void ReqQryInstrumet(char * pInstrument);

	///请求确认结算单
	void ReqSettlementInfoConfirm();

	void OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	
	//是否出现错误
	bool IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo);
	
	///登录请求响应
	void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
	void OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	
	//请求查询结算信息确认响应
	void OnRspQrySettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

	//请求查询投资者结算结果响应
	void OnRspQrySettlementInfo(CThostFtdcSettlementInfoField* pSettlementInfo,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

	//投资者结算结果确认响应
	void OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

	///用户口令更新请求响应
	void OnRspUserPasswordUpdate(CThostFtdcUserPasswordUpdateField* pUserPasswordUpdate,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///请求查询行情响应
	void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	//请求查询投资者持仓响应
	void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField* pInvestorPosition,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;
	//请求查询投资者持仓响应
	void OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

	///请求查询成交响应
	void OnRspQryOrder(CThostFtdcOrderField* pOrder,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

	///请求查询成交响应
	void OnRspQryTrade(CThostFtdcTradeField* pTrade,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

	///请求查询持仓明细响应
	void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField* pField,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;

	//查询资金帐户响应
	void OnRspQryTradingAccount(CThostFtdcTradingAccountField* pTradingAccount,
		CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) override;
	
	///报单录入请求响应，这个是期货公司ctp柜台给我们的回应
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///报单错误请求响应，这个是期货公司ctp柜台给我们的回应
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo);
	
	///报单操作请求响应,撤单的错误响应，期货公司ctp柜台给的回应，本程序下单错误的响应
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField* pInputOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);
	
	///报单操作错误回报，期货公司ctp柜台给的回应，本账户的其他链接的下单错误响应
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField* pOrderAction, CThostFtdcRspInfoField* pRspInfo);

	
	///报单通知
	virtual void OnRtnOrder(CThostFtdcOrderField* pOrder);

	///成交通知
	virtual void OnRtnTrade(CThostFtdcTradeField* pTrade);
	
	//make an order;
	CThostFtdcOrderField* PlaceOrder(const char* pszCode, const char* ExchangeID, int nDirection, int nOpenClose, int nVolume, double fPrice);

	//cancel an order;
	void CancelOrder(CThostFtdcOrderField* pOrder);
	
	// 打印持仓信息
	void ShowPosition();
	
	// 平掉账户的仓位
	void ClosePosition();

	//设置程序化策略能否开仓
	void SetAllowOpen(bool isOk);

	//通过brokerOrderSeq获取报单
	CThostFtdcOrderField* GetOrder(int nBrokerOrderSeq);

	CThostFtdcOrderField* GetOrder(CThostFtdcOrderField* pOrder);
	//更新订单状态
	bool UpdateOrder(CThostFtdcOrderField* pOrder);
	bool UpdateOrder(CThostFtdcOrderField* pOrder, std::map<int ,CThostFtdcOrderField*> & orderMap);
	
	void	ShowInstMessage();

	bool FindTrade(CThostFtdcTradeField* pTrade);
	//在tdList中查找pTrade，找到返回true
	bool FindTrade(CThostFtdcTradeField* pTrade, std::vector<CThostFtdcTradeField*> &tdList);
	void InsertTrade(CThostFtdcTradeField* pTrade);
	//把pTrade插入到tdList
	void InsertTrade(CThostFtdcTradeField* pTrade, std::vector<CThostFtdcTradeField*>& tdList);


	void ShowTradeList();

	void ShowTradeList(std::vector<CThostFtdcTradeField*>& tdList);
	void ShowOrderList(std::map<int, CThostFtdcOrderField*> & orderList);

	bool UpdatePositionDetail(CThostFtdcTradeField* pTrade);

	void ShowPositionDetail();

	void UpdatePositionFieldMap(TThostFtdcInstrumentIDType InstrumentID);

	bool FindPositionInst(TThostFtdcInstrumentIDType InstrumentID);

	void InsertPositionInst(TThostFtdcInstrumentIDType InstrumentID);

	//在subscribe_inst_vec中查找合约是否存在
	bool FindInstInSubVec(TThostFtdcInstrumentIDType InstrumentID);
	//订阅单个合约的行情
	void SubscribeMarketData(std::string strInst);
	//更新账户持仓
	void UpdatePosition();

	void ReFreshPositionField(std::string strInst, position_field * pf);

	bool BeUpDatedMDPrice();

	void UpdatePositionMDPrice(CThostFtdcDepthMarketDataField* pDepthMarketData);
	//释放申请的内存空间
	void Release();
	inline bool GetbUpdateMdPrice()
	{
		return bUpdateMdPrice;
	};
	
	bool CompareRule(const CThostFtdcTradeField* _tf, const CThostFtdcTradeField* _curr);
	//用于保存所有期货合约的映射
	std::map<std::string, CThostFtdcInstrumentField*> m_all_instruments_field_map;

	//持仓信息结构体的map
	std::map<std::string, position_field*> m_position_field_map;
	protected:
			int GetNextRequestID();
			char * GetNextOrderRef();

private:
	CThostFtdcTraderApi* m_pUserTDApi_trade;
	CThostFtdcMdApi* m_pUserMDApi_trade;
	MdSpi* m_pUserMDSpi_trade;
	CThostFtdcReqUserLoginField* loginField;
	CThostFtdcReqAuthenticateField* authField;
	int  m_nNextRequestID;
	
	//当天的所有报单,brokerorderseq
	std::map<int,CThostFtdcOrderField*> orderList;
	//当天的所有报单,frontid+sessionid+orderref
	std::map<std::string, CThostFtdcOrderField*> m_mOrders;
	
	//当天的所有成交
	std::vector<CThostFtdcTradeField*> tradeList;
	
	//未平仓多单持仓明细
	std::list<CThostFtdcTradeField*> positionDetailList_NotClosed_Long;
	
	//未平仓的空单持仓明细
	std::list<CThostFtdcTradeField*> positionDetailList_NotClosed_Short;

	
	
	

	


	//本程序的成交ID集合
	std::set<std::string>  m_TradeIDs;

	//本程序的成交vector
	std::vector<CThostFtdcTradeField*> m_Trades;
	
	//本程序的报单集合
	std::map<int, CThostFtdcOrderField*> m_Orders;

	//map<int, CThostFtdcOrderField*> m_CancelingOrders;
	//账户所有合约的平仓盈亏
	double m_CloseProfit;
	
	//账户所有合约的持仓盈亏
	double m_PositionProfit;

	double m_OpenProfit;

	bool m_QryOrder_Once;
	bool m_QryTrade_Once;
	bool m_QryDetail_Once;
	bool m_QryTradingAccount_Once;
	bool m_QryPosition_Once;
	bool m_QryInstrument_Once;
	std::string m_AppId;
	std::string m_AuthCode;
	std::string m_BrokerId;
	std::string m_UserId;
	std::string m_Passwd;
	std::string m_InstId;
	std::string m_InstId1;
	std::string m_InstId2;
	std::string m_Inst_Position;
	std::string m_Instrument_All;

	int m_nFrontID;
	int m_nSessionID;
	bool m_AllowOpen;
	char orderRef[13];
	//char tradingDate[13];
	TThostFtdcDateType	m_cTradingDay;
	
	bool bUpdateMdPrice;

};