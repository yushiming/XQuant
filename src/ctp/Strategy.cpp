#include "Strategy.h"
#include"timer.h"


extern std::mutex m_mutex;
void Strategy::OnTick(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	/*cout << "===========================================" << endl;
	cout << "深度行情" << endl;
	cout << " 交易日:" << pDepthMarketData->TradingDay << endl
		<< "合约代码:" << pDepthMarketData->InstrumentID << endl
		<< "最新价:" << pDepthMarketData->LastPrice << endl
		<< "上次结算价:" << pDepthMarketData->PreSettlementPrice << endl
		<< "昨收盘:" << pDepthMarketData->PreClosePrice << endl
		<< "数量:" << pDepthMarketData->Volume << endl
		<< "昨持仓量:" << pDepthMarketData->PreOpenInterest << endl
		<< "最后更新时间" << pDepthMarketData->UpdateTime << endl
		<< "最后更新毫秒" << pDepthMarketData->UpdateMillisec << endl
	<< "申买价一：" << pDepthMarketData->BidPrice1 << endl
	<< "申买量一:" << pDepthMarketData->BidVolume1 << endl
	<< "申卖价一:" << pDepthMarketData->AskPrice1 << endl
	<< "申卖量一:" << pDepthMarketData->AskVolume1 << endl
	<< "今收盘价:" << pDepthMarketData->ClosePrice << endl
	<< "当日均价:" << pDepthMarketData->AveragePrice << endl
	<< "本次结算价格:" << pDepthMarketData->SettlementPrice << endl
	<< "成交金额:" << pDepthMarketData->Turnover << endl
	<< "持仓量:" << pDepthMarketData->OpenInterest << endl;*/

	if (!pDepthMarketData )
		return;
	
	// strcmp(m_instId, pDepthMarketData->InstrumentID) != 0
	//记录深度行情数据
	if (strcmp(m_instId1, pDepthMarketData->InstrumentID) == 0 || strcmp(m_instId2, pDepthMarketData->InstrumentID) == 0)
	UpdateDepthMdList(pDepthMarketData);
	
	
	//计算买卖信号并发单，一定要进行合约过滤，我们在主力合约里面下单，否则会出现重复下单的情况
	if(strcmp(m_instId2, pDepthMarketData->InstrumentID) == 0)
	CalculateBuySellSignal();
}

void Strategy::OnStrategyStart()
{
	tickCount = 0;
	//持仓信息，更新两个合约的持仓信息
	m_posfield1.instId = m_instId1;
	m_posfield2.instId = m_instId2;


	UpdatePostionFieldStgy();
	
	
		m_fMinMove1 = 0;
		auto it1 = m_instField_map.find(m_instId1);
		if (it1 != m_instField_map.end())
		{
			m_fMinMove1 = (it1->second)->PriceTick;
		}

		m_fMinMove2 = 0;
		auto it2 = m_instField_map.find(m_instId2);
		if (it2 != m_instField_map.end())
		{
			m_fMinMove2 = (it2->second)->PriceTick;
		}


	//记录开仓委托
	m_pOpenOrder1 = nullptr;
	//记录平仓委托
	m_pCloseOrder1 = nullptr;

	//记录开仓委托
	m_pOpenOrder2 = nullptr;
	//记录平仓委托
	m_pCloseOrder2 = nullptr;

	
}

void Strategy::OnStrategyEnd()
{
	//记录开仓委托
	m_pOpenOrder1 = nullptr;
	//记录平仓委托
	m_pCloseOrder1 = nullptr;

	//记录开仓委托
	m_pOpenOrder2 = nullptr;
	//记录平仓委托
	m_pCloseOrder2 = nullptr;
}

void Strategy::OnBar(CKBar* pBar, CKBarSeries* pBarSeries)
{
	pBar->ShowBar();
}

void Strategy::OnRtnOrder(CThostFtdcOrderField* pOrder)
{
	//是否是我们需要的合约
	if (!pOrder || (strcmp(m_instId1, pOrder->InstrumentID) != 0 && strcmp(m_instId2, pOrder->InstrumentID)!=0))
		return;

	//合约2完全成交
	if (pOrder->OrderStatus == '0'&& strcmp(m_instId2, pOrder->InstrumentID) == 0)
	{
		if (pOrder->CombOffsetFlag[0] == '0')
		{
			m_pOpenOrder1 = nullptr;
			m_pOpenOrder2 = nullptr;
		}

		else
		{
			m_pCloseOrder1 = nullptr;
			m_pCloseOrder2 = nullptr;
		}

	}
	//如果是撤单
	if (pOrder->OrderStatus == '5')
	{
	
		/*if (strcmp(m_instId1, pOrder->InstrumentID) == 0)
		{
			if (pOrder->CombOffsetFlag[0] == '0')
				m_pOpenOrder1 = nullptr;
			else
				m_pCloseOrder1 = nullptr;
		}
		else */
		if (strcmp(m_instId2, pOrder->InstrumentID) == 0)
		{
			
			if (pOrder->CombOffsetFlag[0] == '0')
			{
				//m_pOpenOrder1 = nullptr;
				m_pOpenOrder2 = nullptr;
			}
				
			else
			{
				//m_pCloseOrder1 = nullptr;
				m_pCloseOrder2 = nullptr;
			}
				
		}

	}
	
	
}

void Strategy::OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder)
{
	if (strcmp(pInputOrder->InstrumentID, m_instId1) == 0)
	{
		
		if (pInputOrder->CombOffsetFlag[0] == '0')
			m_pOpenOrder1 = nullptr;
		else
			m_pCloseOrder1 = nullptr;
	}
	

	if (strcmp(pInputOrder->InstrumentID, m_instId2) == 0)
		
	{
		
		if (pInputOrder->CombOffsetFlag[0] == '0')
			m_pOpenOrder2 = nullptr;
		else
			m_pCloseOrder2 = nullptr;
	}
	
}

void Strategy::OnRtnTrade(CThostFtdcTradeField* pTrade)
{
	if (!pTrade)
		return;
	if (strcmp(m_instId1, pTrade->InstrumentID) != 0 && strcmp(m_instId2, pTrade->InstrumentID)!=0)
		return;
	UpdatePostionFieldStgy();
	
		/*if (strcmp(m_instId1, pTrade->InstrumentID) == 0)
		{
			if (pTrade->OffsetFlag == '0')
				m_pOpenOrder1 = nullptr;
			else
				m_pCloseOrder1 = nullptr;
		}
		else */
		/*if (strcmp(m_instId2, pTrade->InstrumentID) == 0)
		{
			if (pTrade->OffsetFlag == '0')
			{
				m_pOpenOrder1 = nullptr;
				m_pOpenOrder2 = nullptr;
			}
				
			else
			{
				m_pCloseOrder1 = nullptr;
				m_pCloseOrder2 = nullptr;
			}
				
		}*/

	//如果是合约2成交，return返回
	if (strcmp(m_instId2, pTrade->InstrumentID) == 0)
		return;
	//还有挂单，先不操作，等撤完单再下一步
	if (!(IsFinished(m_pOpenOrder1) && IsFinished(m_pOpenOrder2) && IsFinished(m_pCloseOrder1) && IsFinished(m_pCloseOrder2)))
		return;
	//	CThostFtdcDepthMarketDataField * pDepth1 = m_pDethMDList1.back();
	CThostFtdcDepthMarketDataField* pDepth2 = m_pDethMDList2.back();
	
	

	//不活跃合约1成交
	if (strcmp(m_instId1, pTrade->InstrumentID) == 0)
	{
		
		//合约1买入平仓,合约2卖出平仓
		if (pTrade->OffsetFlag!='0' &&pTrade->Direction=='0')
		{
			if (m_posfield2.LongPosition * Multi1 > m_posfield1.ShortPosition * Multi2)//不活跃合约1空单，活跃合约2多单
			{
				if (m_pCloseOrder1 && !m_pCloseOrder2)
				{
			
					int lots = (m_posfield2.LongPosition / Multi2 - m_posfield1.ShortPosition / Multi1) * Multi2;
					CThostFtdcDepthMarketDataField*  pDepth2=m_pDethMDList2.back();
					//价格需要处理下，不能超出涨跌停
					double price =  pDepth2->BidPrice1 - 2 * m_fMinMove1;
					if (price > pDepth2->UpperLimitPrice)
						price = pDepth2->UpperLimitPrice;
					else if (price < pDepth2->LowerLimitPrice)
						price = pDepth2->LowerLimitPrice;
					
					m_pCloseOrder2 = Sell(pDepth2->InstrumentID, pDepth2->ExchangeID, lots, price);
					
					std::cerr << "OnRtnTrade Sell Inst2: " << price << endl;
					RegisterTimer(300, 1l, m_pCloseOrder2);
					return;
				}
			}
		}
		
			//还有挂单，先不操作，等撤完单再下一步
		if (!(IsFinished(m_pOpenOrder1) && IsFinished(m_pOpenOrder2) && IsFinished(m_pCloseOrder1) && IsFinished(m_pCloseOrder2)))
			return;
		//合约1卖出平仓,合约2买入平仓,
		if (pTrade->OffsetFlag != '0' && pTrade->Direction == '1')
		{
			if (m_posfield1.LongPosition * Multi2 < m_posfield2.ShortPosition * Multi1)//不活跃合约1空单，活跃合约2多单
			{
				if (m_pCloseOrder1 && !m_pCloseOrder2)
				{
				
					int lots = (m_posfield2.ShortPosition / Multi2 - m_posfield1.LongPosition / Multi1) * Multi2;
					CThostFtdcDepthMarketDataField* pDepth2 = m_pDethMDList2.back();
					//价格需要处理下，不能超出涨跌停
					double price = pDepth2->AskPrice1 + 2 * m_fMinMove1;
					if (price > pDepth2->UpperLimitPrice)
						price = pDepth2->UpperLimitPrice;
					else if (price < pDepth2->LowerLimitPrice)
						price = pDepth2->LowerLimitPrice;
					
					m_pCloseOrder2 = BuytoCover(pDepth2->InstrumentID, pDepth2->ExchangeID, lots, price);
					
					std::cerr << "OnRtnTrade BuytoCover Inst2: " << price << endl;
					RegisterTimer(300, 1l, m_pCloseOrder2);
					return;
				}
			}
		}
		
			//还有挂单，先不操作，等撤完单再下一步
		if (!(IsFinished(m_pOpenOrder1) && IsFinished(m_pOpenOrder2) && IsFinished(m_pCloseOrder1) && IsFinished(m_pCloseOrder2)))
			return;
		//不活跃合约1，卖出开仓，活跃合约2买入开仓
		if (pTrade->OffsetFlag == '0' && pTrade->Direction == '1')
		{
			if ( m_posfield2.LongPosition < totalQty * Multi2 && m_posfield2.LongPosition * Multi1 < m_posfield1.ShortPosition * Multi2)
			//不活跃合约空单，活跃合约多单
			{
				if (m_pOpenOrder1 && !m_pOpenOrder2)
				{
					
					//价格需要处理下，不能超出涨跌停
					CThostFtdcDepthMarketDataField* pDepth2 = m_pDethMDList2.back();
					double price = pDepth2->AskPrice1 + 2 * m_fMinMove1;
					if (price > pDepth2->UpperLimitPrice)
						price = pDepth2->UpperLimitPrice;
					else if (price < pDepth2->LowerLimitPrice)
						price = pDepth2->LowerLimitPrice;
					int lots = (m_posfield1.ShortPosition / Multi1 - m_posfield2.LongPosition / Multi2) * Multi2;
					
					m_pOpenOrder2 = Buy(pDepth2->InstrumentID, pDepth2->ExchangeID, lots, price);
					
					std::cerr << "OnRtnTrade Buy Inst2: " << price << endl; 
					RegisterTimer(300, 1l, m_pOpenOrder2);
					return;
				}


			}
		}

		//还有挂单，先不操作，等撤完单再下一步
		/*if (bePendingOrder)
			return;*/
			//还有挂单，先不操作，等撤完单再下一步
		if (!(IsFinished(m_pOpenOrder1) && IsFinished(m_pOpenOrder2) && IsFinished(m_pCloseOrder1) && IsFinished(m_pCloseOrder2)))
			return;
		//不活跃合约1买入开仓，合约2卖出开仓
		if (pTrade->OffsetFlag == '0' && pTrade->Direction == '0')
		{
			if (m_posfield2.ShortPosition < totalQty * Multi2 &&  m_posfield2.ShortPosition * Multi1 < m_posfield1.LongPosition * Multi2 )
			//不活跃合约空单，活跃合约多单
			{
				if (m_pOpenOrder1 && !m_pOpenOrder2)
				{
					
					int lots = (m_posfield1.LongPosition / Multi1 - m_posfield2.ShortPosition / Multi2) * Multi2;
					//价格需要处理下，不能超出涨跌停
					CThostFtdcDepthMarketDataField* pDepth2 = m_pDethMDList2.back();
					double price = pDepth2->BidPrice1-2*m_fMinMove1;
					if (price > pDepth2->UpperLimitPrice)
						price = pDepth2->UpperLimitPrice;
					else if (price < pDepth2->LowerLimitPrice)
						price = pDepth2->LowerLimitPrice;
					
					m_pOpenOrder2 = Short(pDepth2->InstrumentID, pDepth2->ExchangeID, lots, price);
					
					std::cerr << "OnRtnTrade Short Inst2: " << price<<endl;
					RegisterTimer(300, 1l, m_pOpenOrder2);
					return;
				}


			}
		}

	}
	
	
	
	

}

void Strategy::CancelOrder(CThostFtdcOrderField* pOrder)
{
	CThostFtdcOrderField* pOrder1 = GetOrder(pOrder);
	if (pOrder1 != nullptr && pOrder1->OrderStatus != '0' && pOrder1->OrderStatus != '5')
		m_pUserTDSpi_stgy->CancelOrder(pOrder1);
}

CThostFtdcOrderField* Strategy::GetOrder(CThostFtdcOrderField* pOrder)
{
	if (!pOrder)
		return nullptr;
	
	return m_pUserTDSpi_stgy->GetOrder(pOrder);;
}

void Strategy::RegisterTimer(int milliSeconds, int nAction, CThostFtdcOrderField* pOrder)
{
	
	Timer t1;
	t1.startOnce(milliSeconds,std::bind(&Strategy::OnTimer,this,pOrder,nAction));
}

void Strategy::OnTimer(CThostFtdcOrderField* pOrder, long lData)
{
	std::cerr << "trigger OnTimer" << std::endl;
	CancelOrder(pOrder);

	
}
//报单是否完结，已成交或者已撤单
bool Strategy::IsFinished(CThostFtdcOrderField* pOrder)
{
	if (pOrder == nullptr)
		return true;
	CThostFtdcOrderField* pOrder1 = GetOrder(pOrder);
	if (pOrder1 != nullptr && (pOrder1->OrderStatus == '0' || pOrder1->OrderStatus == '5'))
	return true;
	return false;
}

CThostFtdcOrderField* Strategy::Buy(const char* InstrumentID, const char* ExchangeID, int nVolume, double fPrice)
{
	return m_pUserTDSpi_stgy->PlaceOrder(InstrumentID,ExchangeID,0,0,nVolume,fPrice);
}

CThostFtdcOrderField* Strategy::Sell(const char* InstrumentID, const char* ExchangeID, int nVolume, double fPrice, int YdorToday)
{
	return m_pUserTDSpi_stgy->PlaceOrder(InstrumentID, ExchangeID, 1, YdorToday, nVolume, fPrice);
}

CThostFtdcOrderField* Strategy::Short(const char* InstrumentID, const char* ExchangeID, int nVolume, double fPrice)
{
	return m_pUserTDSpi_stgy->PlaceOrder(InstrumentID, ExchangeID, 1, 0, nVolume, fPrice);
}

CThostFtdcOrderField* Strategy::BuytoCover(const char* InstrumentID, const char* ExchangeID, int nVolume, double fPrice, int YdorToday)
{
	return m_pUserTDSpi_stgy->PlaceOrder(InstrumentID, ExchangeID, 0, YdorToday, nVolume, fPrice);
}

void Strategy::set_instPostion_map_stgy(std::map<std::string, CThostFtdcInstrumentField*> inst_map)
{
	auto it1 = inst_map.find(m_instId1);
	if (it1 != inst_map.end())
	{
		auto it0 = m_instField_map.find(m_instId1);
		auto end0 = m_instField_map.end();
		if (it0 == end0)
			m_instField_map.insert(make_pair(it1->first, it1->second));
	}


	auto it2 = inst_map.find(m_instId2);
	if (it2 != inst_map.end())
	{
		auto it0 = m_instField_map.find(m_instId2);
		auto end0 = m_instField_map.end();
		if (it0 == end0)
			m_instField_map.insert(make_pair(it2->first, it2->second));
	}
}
void Strategy::UpdatePostionFieldStgy()
{
	position_field* p1 = GetPositionField(m_instId1);
	position_field* p2 = GetPositionField(m_instId2);
	if(p1)
	m_posfield1 = *p1;
	if(p2)
	m_posfield2 = *p2;
}


position_field* Strategy::GetPositionField(string instid)
{
	auto it = (m_pUserTDSpi_stgy->m_position_field_map).find(instid);
	if (it != (m_pUserTDSpi_stgy->m_position_field_map).end())
	{
		return it->second;
	}
	
	return nullptr;
}

void Strategy::UpdateDepthMdList(CThostFtdcDepthMarketDataField* pDepth)
{
	if (!pDepth)
		return;
	if (strcmp(pDepth->InstrumentID, m_instId1) == 0)
	{
		CThostFtdcDepthMarketDataField* pDepthMD = new CThostFtdcDepthMarketDataField;
		memcpy(pDepthMD, pDepth, sizeof(CThostFtdcDepthMarketDataField));
		m_pDethMDList1.push_back(pDepthMD);
		if (m_pDethMDList1.size() > 30)
		{
			pDepthMD = m_pDethMDList1.front();
			m_pDethMDList1.pop_front();
			delete pDepthMD;
			pDepthMD = nullptr;
			
		}
			
	}
	else if (strcmp(pDepth->InstrumentID, m_instId2) == 0)
	{
		CThostFtdcDepthMarketDataField* pDepthMD = new CThostFtdcDepthMarketDataField;
		memcpy(pDepthMD, pDepth, sizeof(CThostFtdcDepthMarketDataField));
		m_pDethMDList2.push_back(pDepthMD);
		if (m_pDethMDList2.size() > 30)
		{
			pDepthMD = m_pDethMDList2.front();
			m_pDethMDList2.pop_front();
			delete pDepthMD;
			pDepthMD = nullptr;
		}
			
		
		//计算价差
		if (m_pDethMDList1.size() > 0 && m_pDethMDList2.size() > 0)
		{
			if (strcmp(m_pDethMDList1.back()->UpdateTime, m_pDethMDList2.back()->UpdateTime) == 0 && m_pDethMDList1.back()->UpdateMillisec == m_pDethMDList2.back()->UpdateMillisec)
			{
				Tick* pTick = new Tick;
				pTick->askPrice = m_pDethMDList1.back()->AskPrice1 * Multi1 - m_pDethMDList2.back()->BidPrice1 * Multi2;

				pTick->askVolume = m_pDethMDList1.back()->AskVolume1 / Multi1;

				pTick->bidPrice = m_pDethMDList1.back()->BidPrice1 * Multi1 - m_pDethMDList2.back()->AskPrice1 * Multi2;

				pTick->bidVolume = m_pDethMDList1.back()->BidVolume1 / Multi1;
				pTick->date = m_pDethMDList1.back()->TradingDay;
				pTick->time = m_pDethMDList1.back()->UpdateTime;
				pTick->UpdateMillisec = m_pDethMDList1.back()->UpdateMillisec;
				m_pSpreadMDList.push_back(pTick);
				if (m_pSpreadMDList.size() > 30)
				{
					pTick = m_pSpreadMDList.front();
					m_pSpreadMDList.pop_front();
					std::cerr << "ask:"<<pTick->askPrice<< "  askvol:" << pTick->askVolume << " bid:"<< pTick->bidPrice<<" bidvol:" << pTick->bidVolume<<endl;
					delete pTick;
					pTick = nullptr;
				}
			
			}
			
				
		}
			
	}

	
}


void Strategy::CalculateProfitInfo(CThostFtdcDepthMarketDataField* pDethMD)
{
}

void Strategy::SaveTickToVec(CThostFtdcDepthMarketDataField* pDethMD)
{
}

void Strategy::SaveTickToTxtCsv(CThostFtdcDepthMarketDataField* pDethMD)
{
}

//计算开平仓信号
void Strategy::CalculateBuySellSignal()
{
	//行情是否为空
	if (m_pSpreadMDList.size()==0)
		return ;
	
	Tick* pTick = m_pSpreadMDList.back();
	
	CThostFtdcDepthMarketDataField* pDepth1=m_pDethMDList1.back();
	CThostFtdcDepthMarketDataField* pDepth2 = m_pDethMDList2.back();
	
	
	//合约1,不活跃合约，是否有挂单，价差不满足撤单
	if (IsFinished(m_pOpenOrder1) == false)
	{
		//合约1卖出开仓撤单
		if ( m_pOpenOrder1->Direction == '1' && m_pOpenOrder1->CombOffsetFlag[0] == '0' && pTick->bidPrice < ShortEntryLine )
		{
			CancelOrder(m_pOpenOrder1);
		}
		//合约1买入开仓撤单
		if (m_pOpenOrder1->Direction == '0' && m_pOpenOrder1->CombOffsetFlag[0] == '0' && pTick->askPrice > LongEntryLine)
		{
			CancelOrder(m_pOpenOrder1);
		}
	}
	
	if (IsFinished(m_pCloseOrder1) == false)
	{
		//合约1卖出平仓撤单
		if (m_pCloseOrder1->Direction == '1' && m_pCloseOrder1->CombOffsetFlag[0] != '0' && pTick->bidPrice > ShortExitLine && pTick->bidPrice < StopShortLine)
		{
			CancelOrder(m_pCloseOrder1);
		}
		//合约1买入平仓撤单
		if (m_pCloseOrder1->Direction == '0' && m_pCloseOrder1->CombOffsetFlag[0] != '0' && pTick->askPrice < LongExitLine && pTick->askPrice > StopLongLine)
		{
			CancelOrder(m_pCloseOrder1);
		}
	}
	//还有挂单，先不操作，等撤完单再下一步
	if (!(IsFinished(m_pOpenOrder1) && IsFinished(m_pOpenOrder2) && IsFinished(m_pCloseOrder1) && IsFinished(m_pCloseOrder2)))
		return;
	
	//合约1买入平仓
	if (!m_pCloseOrder1 && !m_pCloseOrder2 && m_posfield1.ShortPosition > 0 )
	{
		if (m_posfield1.ShortPosition * Multi2 > (m_posfield2.LongPosition - Qty * Multi2) * Multi1)

		{
			if (pTick->askPrice - ShortExitLine <= 0 || pTick->askPrice - StopShortLine >= 0)
			{
				
				//价格需要处理下，不能超出涨跌停
				double price = min(pDepth1->BidPrice1 + 2 * m_fMinMove2, pDepth1->AskPrice1);
				//下单手数
				int lots = (Qty+m_posfield1.ShortPosition/Multi1  - m_posfield2.LongPosition / Multi2)*Multi1;
				if (price > pDepth1->UpperLimitPrice)
					price = pDepth1->UpperLimitPrice;
				else if (price < pDepth1->LowerLimitPrice)
					price = pDepth1->LowerLimitPrice;
				
				m_pCloseOrder1 = BuytoCover(pDepth1->InstrumentID, pDepth1->ExchangeID, lots, price);
				
				std::cerr << "Signal BuytoCover Inst1: " << price<<endl;
				RegisterTimer(300, 1l, m_pCloseOrder1);
				return;
			}
			
		}
	}
	
		//还有挂单，先不操作，等撤完单再下一步
	if (!(IsFinished(m_pOpenOrder1) && IsFinished(m_pOpenOrder2) && IsFinished(m_pCloseOrder1) && IsFinished(m_pCloseOrder2)))
		return;
	//合约1卖出平仓
	if (m_posfield1.LongPosition > 0 )
	{
		if (m_posfield1.LongPosition * Multi2 == m_posfield2.ShortPosition  * Multi1)
		{
			if (pTick->bidPrice - LongExitLine >= 0 || pTick->bidPrice - StopLongLine <= 0)
			{
			
				if (!m_pCloseOrder1 && !m_pCloseOrder2 )
				{
					//价格需要处理下，不能超出涨跌停
					double price = max(pDepth1->AskPrice1 - 2 * m_fMinMove2, pDepth1->BidPrice1);
					int lots = (Qty + m_posfield2.ShortPosition / Multi2 - m_posfield1.LongPosition / Multi1) * Multi2;
					if (price > pDepth1->UpperLimitPrice)
						price = pDepth1->UpperLimitPrice;
					else if (price < pDepth1->LowerLimitPrice)
						price = pDepth1->LowerLimitPrice;

					m_pCloseOrder1 = Sell(pDepth1->InstrumentID, pDepth1->ExchangeID, lots, price);

					std::cerr << "Signal Sell Inst1: " << price << endl;
					RegisterTimer(300, 1l, m_pCloseOrder1);
					return;
				}
			}

		}
	}
	
		//还有挂单，先不操作，等撤完单再下一步
	if (!(IsFinished(m_pOpenOrder1) && IsFinished(m_pOpenOrder2) && IsFinished(m_pCloseOrder1) && IsFinished(m_pCloseOrder2)))
		return;
	//合约1卖出开仓
	if ( m_posfield1.LongPosition == 0 && m_posfield1.ShortPosition < totalQty * Multi1 && (m_posfield1.ShortPosition / Multi1) == ( m_posfield2.LongPosition / Multi2) )
	{
		if (pTick->bidPrice > ShortEntryLine)
		{
			if (!m_pOpenOrder1 && !m_pOpenOrder2)
			{
				//价格需要处理下，不能超出涨跌停
				//double price = pDepth1->BidPrice1 - 2 * m_fMinMove2;
				double price = max(pDepth1->AskPrice1 - 2 * m_fMinMove2, pDepth1->BidPrice1);
				int lots = (Qty + m_posfield2.LongPosition / Multi2 - m_posfield1.ShortPosition / Multi1) * Multi1;
				if (price > pDepth1->UpperLimitPrice)
					price = pDepth1->UpperLimitPrice;
				else if (price < pDepth1->LowerLimitPrice)
					price = pDepth1->LowerLimitPrice;

				m_pOpenOrder1 = Short(pDepth1->InstrumentID, pDepth1->ExchangeID, lots, price);

				std::cerr << "Signal Short Inst1: " << price << endl;
				RegisterTimer(300, 1l, m_pOpenOrder1);
				return;
			}
		}
	}
	
		//还有挂单，先不操作，等撤完单再下一步
	if (!(IsFinished(m_pOpenOrder1) && IsFinished(m_pOpenOrder2) && IsFinished(m_pCloseOrder1) && IsFinished(m_pCloseOrder2)))
		return;
	//合约1买入开仓
	if ( m_posfield1.ShortPosition == 0 && m_posfield1.LongPosition < totalQty * Multi1 && (m_posfield1.LongPosition / Multi1) ==  (m_posfield2.ShortPosition / Multi2) )
	{
		if (pTick->askPrice < LongEntryLine)
		{
			if (!m_pOpenOrder1 && !m_pOpenOrder2)
			{
				//价格需要处理下，不能超出涨跌停
			//double price = pDepth1->BidPrice1 + 2 * m_fMinMove2;
				double price = min(pDepth1->AskPrice1, pDepth1->BidPrice1 + 2 * m_fMinMove2);
				int lots = (Qty + m_posfield2.ShortPosition / Multi2 - m_posfield1.LongPosition / Multi1) * Multi1;
				if (price > pDepth1->UpperLimitPrice)
					price = pDepth1->UpperLimitPrice;
				else if (price < pDepth1->LowerLimitPrice)
					price = pDepth1->LowerLimitPrice;

				m_pOpenOrder1 = Buy(pDepth1->InstrumentID, pDepth1->ExchangeID, lots, price);

				std::cerr << "Signal Buy Inst1: " << price << endl;
				RegisterTimer(300, 1l, m_pOpenOrder1);
				//cerr << "buy " << pDepth1->InstrumentID << " ontick" << endl;
				return;
			}
			
		}
	}

		//还有挂单，先不操作，等撤完单再下一步
	if (!(IsFinished(m_pOpenOrder1) && IsFinished(m_pOpenOrder2) && IsFinished(m_pCloseOrder1) && IsFinished(m_pCloseOrder2)))
		return;
	//合约2卖出平仓
	if (m_posfield2.LongPosition > 0 )
	{
		if ( m_posfield2.LongPosition * Multi1 > m_posfield1.ShortPosition * Multi2)//不活跃合约1空单，活跃合约2多单
		{
			
			if (!m_pCloseOrder2 && m_pCloseOrder1)
			{

				int lots = (m_posfield2.LongPosition / Multi2 - m_posfield1.ShortPosition / Multi1) * Multi2;
				//价格需要处理下，不能超出涨跌停
				double price = pDepth2->BidPrice1 - OffsetTicks * m_fMinMove1;
				if (price > pDepth2->UpperLimitPrice)
					price = pDepth2->UpperLimitPrice;
				else if (price < pDepth2->LowerLimitPrice)
					price = pDepth2->LowerLimitPrice;

				m_pCloseOrder2 = Sell(pDepth2->InstrumentID, pDepth2->ExchangeID, lots, price);

				std::cerr << "Signal Sell Inst2: " << price << endl;
				RegisterTimer(300, 1l, m_pCloseOrder2);
				return;
			}
		}
	}
	
		//还有挂单，先不操作，等撤完单再下一步
	if (!(IsFinished(m_pOpenOrder1) && IsFinished(m_pOpenOrder2) && IsFinished(m_pCloseOrder1) && IsFinished(m_pCloseOrder2)))
		return;
	//合约2买入平仓
	if (m_posfield2.ShortPosition > 0 )
	{
		if ( m_posfield2.ShortPosition * Multi1 > m_posfield1.LongPosition * Multi2)//不活跃合约1空单，活跃合约2多单
		{
			if (!m_pCloseOrder2 && m_pCloseOrder1)
			{
				
				int lots = (m_posfield2.ShortPosition / Multi2 - m_posfield1.LongPosition / Multi1) * Multi2;
				//价格需要处理下，不能超出涨跌停
				double price = pDepth2->AskPrice1 + 2 * m_fMinMove1;
				if (price > pDepth2->UpperLimitPrice)
					price = pDepth2->UpperLimitPrice;
				else if (price < pDepth2->LowerLimitPrice)
					price = pDepth2->LowerLimitPrice;
				
				m_pCloseOrder2 = BuytoCover(pDepth2->InstrumentID, pDepth2->ExchangeID, lots,price );
				
				std::cerr << "Signal BuytoCover Inst2: " << price << endl;
				RegisterTimer(300, 1l, m_pCloseOrder2);
				return;
			}
		}
	}
	
	//还有挂单，先不操作，等撤完单再下一步
	if (!(IsFinished(m_pOpenOrder1) && IsFinished(m_pOpenOrder2) && IsFinished(m_pCloseOrder1) && IsFinished(m_pCloseOrder2)))
		return;
	//活跃合约2买入开仓
	if (m_posfield2.LongPosition < totalQty * Multi2)
		{
			if ( m_posfield2.LongPosition * Multi1 < m_posfield1.ShortPosition * Multi2)//不活跃合约空单，活跃合约多单
			{
				
				if (!m_pOpenOrder2 && m_pOpenOrder1)
				{
					//价格需要处理下，不能超出涨跌停
					double price = pDepth2->AskPrice1 + 2 * m_fMinMove1;
					if (price > pDepth2->UpperLimitPrice)
						price = pDepth2->UpperLimitPrice;
					else if (price < pDepth2->LowerLimitPrice)
						price = pDepth2->LowerLimitPrice;
					int lots = (m_posfield1.ShortPosition / Multi1 - m_posfield2.LongPosition / Multi2) * Multi2;

					m_pOpenOrder2 = Buy(pDepth2->InstrumentID, pDepth2->ExchangeID, lots, price);

					std::cerr << "Signal Buy Inst2: " << price << endl;
					RegisterTimer(300, 1l, m_pOpenOrder2);
					return;

				}
					

			}
		}
		
	
	//还有挂单，先不操作，等撤完单再下一步
	if (!(IsFinished(m_pOpenOrder1) && IsFinished(m_pOpenOrder2) && IsFinished(m_pCloseOrder1) && IsFinished(m_pCloseOrder2)))
		return;
		//合约2卖出开仓
		if (m_posfield2.ShortPosition < totalQty * Multi2)
		{
			if (m_posfield2.ShortPosition * Multi1 < m_posfield1.LongPosition * Multi2)
				//不活跃合约空单，活跃合约多单
			{
				
				if (!m_pOpenOrder2 && m_pOpenOrder1)
				{
					int lots = (m_posfield1.LongPosition / Multi1 - m_posfield2.ShortPosition / Multi2) * Multi2;
					//价格需要处理下，不能超出涨跌停
					double price = pDepth2->BidPrice1 - 2 * m_fMinMove1;
					if (price > pDepth2->UpperLimitPrice)
						price = pDepth2->UpperLimitPrice;
					else if (price < pDepth2->LowerLimitPrice)
						price = pDepth2->LowerLimitPrice;

					m_pOpenOrder2 = Short(pDepth2->InstrumentID, pDepth2->ExchangeID, lots, price);

					std::cerr << "Signal Short Inst2: " << price << endl;
					RegisterTimer(300, 1l, m_pOpenOrder2);
					return;
				}
			}
		}
	
}
