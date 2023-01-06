#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"TdSpi.h"

extern std::map<std::string, std::string> accountConfig_map;//保存账户信息的map

class Strategy
{
public:
    Strategy(TdSpi* pUserSpi_trade) :m_pUserTDSpi_stgy(pUserSpi_trade)
    {
       
        strcpy(m_instId1, accountConfig_map["contract1"].c_str());//ni2110
        strcpy(m_instId2, accountConfig_map["contract2"].c_str());//ni2111
        tickCount = 0;

       
    }
    //tick事件
    void OnTick(CThostFtdcDepthMarketDataField* pDethMD);
    //策略启动事件
    void OnStrategyStart();
    //策略停止事件
    void OnStrategyEnd();
    //k线事件
    void OnBar(CKBar * pBar,CKBarSeries *pBarSeries);
    //订单状态事件
    void OnRtnOrder(CThostFtdcOrderField* pOrder);
    void OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder);
    //成交事件
    void OnRtnTrade(CThostFtdcTradeField* pTrade);
    //撤单操作
    void CancelOrder(CThostFtdcOrderField* pOrder);
    CThostFtdcOrderField* GetOrder(CThostFtdcOrderField* pOrder);
    //注册计时器，做高频交易时，需要多少毫秒以后不成交撤单，需要计时器
    void RegisterTimer(int milliSeconds, int nAction, CThostFtdcOrderField* pOrder);
    //计时器通知时间到了
    void OnTimer(CThostFtdcOrderField* pOrder, long lData);
    
    bool IsFinished(CThostFtdcOrderField* pOrder);
    //buy open
    CThostFtdcOrderField* Buy(const char* InstrumentID, const char* ExchangeID, int nVolume, double fPrice);

    //sell close
    CThostFtdcOrderField* Sell(const char* InstrumentID, const char* ExchangeID, int nVolume, double fPrice, int YdorToday = 2);

    //sell open;
    CThostFtdcOrderField* Short(const char* InstrumentID, const char* ExchangeID, int nVolume, double fPrice);

    // buy to close;
    CThostFtdcOrderField* BuytoCover(const char* InstrumentID, const char* ExchangeID, int nVolume, double fPrice, int YdorToday = 2);

    //把持仓的合约设置到策略里面
    void set_instPostion_map_stgy(std::map<std::string, CThostFtdcInstrumentField*> inst_map);
   
    //把交易合约设置到策略持仓信息结构体里面
  /*  void set_positionField_map_stgy(std::map<std::string, position_field*> position_field_map);*/
    void UpdatePostionFieldStgy();
   
    
    position_field* GetPositionField(string instid);
    //更新价差数据
    void UpdateDepthMdList(CThostFtdcDepthMarketDataField* pDepth);

private:

    std::map<std::string, CThostFtdcInstrumentField*> m_instField_map;
    TdSpi* m_pUserTDSpi_stgy;
    //计算账户盈亏
    void CalculateProfitInfo(CThostFtdcDepthMarketDataField* pDethMD);
    
    //保存tick数据到vector
    void SaveTickToVec(CThostFtdcDepthMarketDataField* pDethMD);

    //保存tick数据到txt和csv
    void SaveTickToTxtCsv(CThostFtdcDepthMarketDataField* pDethMD);
    
    //计算开仓平仓信号
    void CalculateBuySellSignal();
    
    //交易的合约1,不活跃合约
    char m_instId1[32];
    //交易的合约2，活跃合约
    char m_instId2[32];
  
    //合约1最小变动价位
    double m_fMinMove1;
    //合约2最小变动价位
    double m_fMinMove2;

    //合约1开仓的委托
    CThostFtdcOrderField* m_pOpenOrder1;
    //合约1平仓委托
    CThostFtdcOrderField* m_pCloseOrder1;

    //合约2开仓的委托
    CThostFtdcOrderField* m_pOpenOrder2;
    //合约2平仓委托
    CThostFtdcOrderField* m_pCloseOrder2;

    //开仓时间
    int nStartSeconds;
    //平仓时间
    int nEndSeconds;
   
    int tickCount;
    
    //保存持仓信息,合约1
    position_field m_posfield1;

    //保存持仓信息，合约2
    position_field m_posfield2;
    //保存深度行情
    std::list<CThostFtdcDepthMarketDataField* > m_pDethMDList;

    //保存合约1深度行情
    std::list<CThostFtdcDepthMarketDataField* > m_pDethMDList1;
    //保存合约2深度行情
    std::list<CThostFtdcDepthMarketDataField* > m_pDethMDList2;

    //保存价差深度行情
    std::list<Tick * > m_pSpreadMDList;
    //空头开仓价差
    double ShortEntryLine = -43.2;//上穿
    //空头平仓价差
    double ShortExitLine = -44.2;//下穿

    //空头止损价差
    double StopShortLine = -38.0;//上穿

    //多头开仓价差
    double LongEntryLine = -46.6;//下穿
   //多头平仓价差
    double LongExitLine = -43.2;//上穿
    //多头止损价差
    double StopLongLine = -49.8;//下穿
  
    //套利开仓总套数
    int totalQty = 2;
    //合约1开仓倍数
    const int Multi1 = 1;
    //合约2开仓倍数
    const int Multi2 = 1;

    //下单价格偏移跳数
    int OffsetTicks = 2;
    //每次下单数量
    int Qty = 1;
   

    //合约1卖价
    double Inst1QuoteAsk;
    //合约1买价
    double Inst1QuoteBid;
    //合约2卖价
    double Inst2QuoteAsk;
    //合约2买价
    double Inst2QuoteBid;
};