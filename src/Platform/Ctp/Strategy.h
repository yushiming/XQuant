#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include"TdSpi.h"

extern std::map<std::string, std::string> accountConfig_map;//�����˻���Ϣ��map

class Strategy
{
public:
    Strategy(TdSpi* pUserSpi_trade) :m_pUserTDSpi_stgy(pUserSpi_trade)
    {
       
        strcpy(m_instId1, accountConfig_map["contract1"].c_str());//ni2110
        strcpy(m_instId2, accountConfig_map["contract2"].c_str());//ni2111
        tickCount = 0;

       
    }
    //tick�¼�
    void OnTick(CThostFtdcDepthMarketDataField* pDethMD);
    //���������¼�
    void OnStrategyStart();
    //����ֹͣ�¼�
    void OnStrategyEnd();
    //k���¼�
    void OnBar(CKBar * pBar,CKBarSeries *pBarSeries);
    //����״̬�¼�
    void OnRtnOrder(CThostFtdcOrderField* pOrder);
    void OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder);
    //�ɽ��¼�
    void OnRtnTrade(CThostFtdcTradeField* pTrade);
    //��������
    void CancelOrder(CThostFtdcOrderField* pOrder);
    CThostFtdcOrderField* GetOrder(CThostFtdcOrderField* pOrder);
    //ע���ʱ��������Ƶ����ʱ����Ҫ���ٺ����Ժ󲻳ɽ���������Ҫ��ʱ��
    void RegisterTimer(int milliSeconds, int nAction, CThostFtdcOrderField* pOrder);
    //��ʱ��֪ͨʱ�䵽��
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

    //�ѳֲֵĺ�Լ���õ���������
    void set_instPostion_map_stgy(std::map<std::string, CThostFtdcInstrumentField*> inst_map);
   
    //�ѽ��׺�Լ���õ����Գֲ���Ϣ�ṹ������
  /*  void set_positionField_map_stgy(std::map<std::string, position_field*> position_field_map);*/
    void UpdatePostionFieldStgy();
   
    
    position_field* GetPositionField(string instid);
    //���¼۲�����
    void UpdateDepthMdList(CThostFtdcDepthMarketDataField* pDepth);

private:

    std::map<std::string, CThostFtdcInstrumentField*> m_instField_map;
    TdSpi* m_pUserTDSpi_stgy;
    //�����˻�ӯ��
    void CalculateProfitInfo(CThostFtdcDepthMarketDataField* pDethMD);
    
    //����tick���ݵ�vector
    void SaveTickToVec(CThostFtdcDepthMarketDataField* pDethMD);

    //����tick���ݵ�txt��csv
    void SaveTickToTxtCsv(CThostFtdcDepthMarketDataField* pDethMD);
    
    //���㿪��ƽ���ź�
    void CalculateBuySellSignal();
    
    //���׵ĺ�Լ1,����Ծ��Լ
    char m_instId1[32];
    //���׵ĺ�Լ2����Ծ��Լ
    char m_instId2[32];
  
    //��Լ1��С�䶯��λ
    double m_fMinMove1;
    //��Լ2��С�䶯��λ
    double m_fMinMove2;

    //��Լ1���ֵ�ί��
    CThostFtdcOrderField* m_pOpenOrder1;
    //��Լ1ƽ��ί��
    CThostFtdcOrderField* m_pCloseOrder1;

    //��Լ2���ֵ�ί��
    CThostFtdcOrderField* m_pOpenOrder2;
    //��Լ2ƽ��ί��
    CThostFtdcOrderField* m_pCloseOrder2;

    //����ʱ��
    int nStartSeconds;
    //ƽ��ʱ��
    int nEndSeconds;
   
    int tickCount;
    
    //����ֲ���Ϣ,��Լ1
    position_field m_posfield1;

    //����ֲ���Ϣ����Լ2
    position_field m_posfield2;
    //�����������
    std::list<CThostFtdcDepthMarketDataField* > m_pDethMDList;

    //�����Լ1�������
    std::list<CThostFtdcDepthMarketDataField* > m_pDethMDList1;
    //�����Լ2�������
    std::list<CThostFtdcDepthMarketDataField* > m_pDethMDList2;

    //����۲��������
    std::list<Tick * > m_pSpreadMDList;
    //��ͷ���ּ۲�
    double ShortEntryLine = -43.2;//�ϴ�
    //��ͷƽ�ּ۲�
    double ShortExitLine = -44.2;//�´�

    //��ͷֹ��۲�
    double StopShortLine = -38.0;//�ϴ�

    //��ͷ���ּ۲�
    double LongEntryLine = -46.6;//�´�
   //��ͷƽ�ּ۲�
    double LongExitLine = -43.2;//�ϴ�
    //��ͷֹ��۲�
    double StopLongLine = -49.8;//�´�
  
    //��������������
    int totalQty = 2;
    //��Լ1���ֱ���
    const int Multi1 = 1;
    //��Լ2���ֱ���
    const int Multi2 = 1;

    //�µ��۸�ƫ������
    int OffsetTicks = 2;
    //ÿ���µ�����
    int Qty = 1;
   

    //��Լ1����
    double Inst1QuoteAsk;
    //��Լ1���
    double Inst1QuoteBid;
    //��Լ2����
    double Inst2QuoteAsk;
    //��Լ2���
    double Inst2QuoteBid;
};