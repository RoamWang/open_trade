#pragma once
#include "ThostFtdcTraderApi.h"
#include "spsc_queue.h"

class user_info;
class trade_spi
	: public CThostFtdcTraderSpi
{
public:
	trade_spi(optimized_queue* queue);
	~trade_spi();

	void init();
	void free_api();

	///当客户端与交易后台建立起通信连接时（还未登录前），该方法被调用。
	virtual void OnFrontConnected();

	///当客户端与交易后台通信连接断开时，该方法被调用。当发生这个情况后，API会自动重新连接，客户端可不做处理。
	///@param nReason 错误原因
	///        -3	关闭连接
	///        -4	网络读失败
	///        -5	网络写失败
	///        -6	读订阅流水请求出错
	///        -7	序列号错误
	///        -8	读心跳出错
	///        -9	错误的网络包大小
	virtual void OnFrontDisconnected(int nReason);

	///心跳超时警告。当长时间未收到报文时，该方法被调用。
	///@param nTimeLapse 距离上次接收报文的时间
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///订阅流控警告应答
	virtual void OnRspSubscribeFlowCtrlWarning(CThostFtdcSpecificTraderField* pRspSubscribeTraderField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///取消订阅流控警告应答
	virtual void OnRspUnSubscribeFlowCtrlWarning(CThostFtdcSpecificTraderField* pRspSubscribeTraderField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///客户端认证响应
	virtual void OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///登录请求响应
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///登出请求响应
	virtual void OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///报单录入请求响应
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///报单操作请求响应
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField* pInputOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///做市商批量报单操作请求响应
	virtual void OnRspMKBatchOrderAction(CThostFtdcMKInputOrderActionField* pMKInputOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///执行宣告录入请求响应
	virtual void OnRspExecOrderInsert(CThostFtdcInputExecOrderField* pInputExecOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///执行宣告操作请求响应
	virtual void OnRspExecOrderAction(CThostFtdcInputExecOrderActionField* pInputExecOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///询价录入请求响应
	virtual void OnRspForQuoteInsert(CThostFtdcInputForQuoteField* pInputForQuote, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///报价录入请求响应
	virtual void OnRspQuoteInsert(CThostFtdcInputQuoteField* pInputQuote, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///报价操作请求响应
	virtual void OnRspQuoteAction(CThostFtdcInputQuoteActionField* pInputQuoteAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///批量报单操作请求响应
	virtual void OnRspBatchOrderAction(CThostFtdcInputBatchOrderActionField* pInputBatchOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///期权自对冲录入请求响应
	virtual void OnRspOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField* pInputOptionSelfClose, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///期权自对冲操作请求响应
	virtual void OnRspOptionSelfCloseAction(CThostFtdcInputOptionSelfCloseActionField* pInputOptionSelfCloseAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///申请组合录入请求响应
	virtual void OnRspCombActionInsert(CThostFtdcInputCombActionField* pInputCombAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询报单响应
	virtual void OnRspQryOrder(CThostFtdcOrderField* pOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询成交响应
	virtual void OnRspQryTrade(CThostFtdcTradeField* pTrade, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询投资者持仓响应
	virtual void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField* pInvestorPosition, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询资金账户响应
	virtual void OnRspQryTradingAccount(CThostFtdcTradingAccountField* pTradingAccount, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询投资者响应
	virtual void OnRspQryInvestor(CThostFtdcInvestorField* pInvestor, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询交易编码响应
	virtual void OnRspQryTradingCode(CThostFtdcTradingCodeField* pTradingCode, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询合约保证金率响应
	virtual void OnRspQryInstrumentMarginRate(CThostFtdcInstrumentMarginRateField* pInstrumentMarginRate, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询合约手续费率响应
	virtual void OnRspQryInstrumentCommissionRate(CThostFtdcInstrumentCommissionRateField* pInstrumentCommissionRate, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询交易所响应
	virtual void OnRspQryExchange(CThostFtdcExchangeField* pExchange, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询产品响应
	virtual void OnRspQryProduct(CThostFtdcProductField* pProduct, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询合约响应
	virtual void OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询申请组合合约响应
	virtual void OnRspQryCombInstrument(CThostFtdcCombInstrumentField* pCombInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询投资者RCAMS组合保证金响应
	virtual void OnRspQryRCAMSInvestorProdMargin(CThostFtdcRCAMSInvestorProdMarginField* pRCAMSInvestorProdMargin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询RCAMS策略组合持仓响应
	virtual void OnRspQryRCAMSInvestorCombPosition(CThostFtdcRCAMSInvestorCombPositionField* pRCAMSInvestorCombPosition, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询申请组合响应
	virtual void OnRspQryCombAction(CThostFtdcCombActionField* pCombAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询组合单腿汇总表响应
	virtual void OnRspQryInvestorPositionForComb(CThostFtdcInvestorPositionForCombField* pForComb, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询行情响应
	virtual void OnRspQryDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询合约状态响应
	virtual void OnRspQryInstrumentStatus(CThostFtdcInstrumentStatusField* pInstrumentStatus, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询投资者持仓明细响应
	virtual void OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField* pInvestorPositionDetail, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询交易所保证金率响应
	virtual void OnRspQryExchangeMarginRate(CThostFtdcExchangeMarginRateField* pExchangeMarginRate, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询交易所调整保证金率响应
	virtual void OnRspQryExchangeMarginRateAdjust(CThostFtdcExchangeMarginRateAdjustField* pExchangeMarginRateAdjust, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询期权交易成本响应
	virtual void OnRspQryOptionInstrTradeCost(CThostFtdcOptionInstrTradeCostField* pOptionInstrTradeCost, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询期权合约手续费响应
	virtual void OnRspQryOptionInstrCommRate(CThostFtdcOptionInstrCommRateField* pOptionInstrCommRate, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询执行宣告响应
	virtual void OnRspQryExecOrder(CThostFtdcExecOrderField* pExecOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询询价响应
	virtual void OnRspQryForQuote(CThostFtdcForQuoteField* pForQuote, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询询价价差响应
	virtual void OnRspQryForQuoteParam(CThostFtdcForQuoteParamField* pForQuoteParam, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询投资者SPBM品种明细响应
	virtual void OnRspQryInvestorProdSPBMDetail(CThostFtdcInvestorProdSPBMDetailField* pInvestorProdSPBMDetail, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询投资者SPMM商品群明细响应
	virtual void OnRspQrySPMMInvestorCommodityGroupMargin(CThostFtdcSPMMInvestorCommodityGroupMarginField* pSPMMInvestorCommodityGroupMargin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询投资者RULE保证金响应
	virtual void OnRspQryRULEInvestorProdMargin(CThostFtdcRULEInvestorProdMarginField* pRULEInvestorProdMargin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询交易员报盘机响应
	virtual void OnRspQryTraderOffer(CThostFtdcTraderOfferField* pTraderOffer, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询报价响应
	virtual void OnRspQryQuote(CThostFtdcQuoteField* pQuote, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询期权自对冲响应
	virtual void OnRspQryOptionSelfClose(CThostFtdcOptionSelfCloseField* pOptionSelfClose, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///请求查询交易开关设置响应
	virtual void OnRspQryControlParam(CThostFtdcControlParamField* pControlParam, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///错误应答
	virtual void OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///报单通知
	virtual void OnRtnOrder(CThostFtdcOrderField* pOrder) {};

	///成交通知
	virtual void OnRtnTrade(CThostFtdcTradeField* pTrade) {};

	///报单录入错误回报
	virtual void OnErrRtnOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo) {};

	///报单操作错误回报
	virtual void OnErrRtnOrderAction(CThostFtdcOrderActionField* pOrderAction, CThostFtdcRspInfoField* pRspInfo) {};

	///合约交易状态通知
	virtual void OnRtnInstrumentStatus(CThostFtdcInstrumentStatusField* pInstrumentStatus) {};

	///执行宣告通知
	virtual void OnRtnExecOrder(CThostFtdcExecOrderField* pExecOrder) {};

	///执行宣告录入错误回报
	virtual void OnErrRtnExecOrderInsert(CThostFtdcInputExecOrderField* pInputExecOrder, CThostFtdcRspInfoField* pRspInfo) {};

	///执行宣告操作错误回报
	virtual void OnErrRtnExecOrderAction(CThostFtdcExecOrderActionField* pExecOrderAction, CThostFtdcRspInfoField* pRspInfo) {};

	///询价录入错误回报
	virtual void OnErrRtnForQuoteInsert(CThostFtdcInputForQuoteField* pInputForQuote, CThostFtdcRspInfoField* pRspInfo) {};

	///报价通知
	virtual void OnRtnQuote(CThostFtdcQuoteField* pQuote) {};

	///报价录入错误回报
	virtual void OnErrRtnQuoteInsert(CThostFtdcInputQuoteField* pInputQuote, CThostFtdcRspInfoField* pRspInfo) {};

	///报价操作错误回报
	virtual void OnErrRtnQuoteAction(CThostFtdcQuoteActionField* pQuoteAction, CThostFtdcRspInfoField* pRspInfo) {};

	///询价通知
	virtual void OnRtnForQuoteRsp(CThostFtdcForQuoteRspField* pForQuoteRsp) {};

	///批量报单操作错误回报
	virtual void OnErrRtnBatchOrderAction(CThostFtdcBatchOrderActionField* pBatchOrderAction, CThostFtdcRspInfoField* pRspInfo) {};

	///期权自对冲通知
	virtual void OnRtnOptionSelfClose(CThostFtdcOptionSelfCloseField* pOptionSelfClose) {};

	///期权自对冲录入错误回报
	virtual void OnErrRtnOptionSelfCloseInsert(CThostFtdcInputOptionSelfCloseField* pInputOptionSelfClose, CThostFtdcRspInfoField* pRspInfo) {};

	///期权自对冲操作错误回报
	virtual void OnErrRtnOptionSelfCloseAction(CThostFtdcOptionSelfCloseActionField* pOptionSelfCloseAction, CThostFtdcRspInfoField* pRspInfo) {};

	///申请组合通知
	virtual void OnRtnCombAction(CThostFtdcCombActionField* pCombAction) {};

	///请求查询申报费响应
	virtual void OnRspQryInstrumentOrderCommRate(CThostFtdcInstrumentOrderCommRateField* pInstrumentOrderCommRate, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///交易所席位流控警告
	virtual void OnRtnFlowCtrlWarning(CThostFtdcFlowCtrlWarningField* pFlowCtrlWarning) {};

	///订阅资金变动应答
	virtual void OnRspSubscribeFundChange(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	///取消订阅资金变动应答
	virtual void OnRspUnSubscribeFundChange(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast) {};

	//资金变动推送
	virtual void OnRtnFundChange(CThostFtdcTradingAccountField* pTradingAccount) {};

private:
	CThostFtdcTraderApi* api_;
	user_info* user_info_;

	optimized_queue* queue_;
};
