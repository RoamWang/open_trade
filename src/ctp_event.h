#pragma once
#include <cstring>
#include "ThostFtdcUserApiStruct.h"

enum class ctp_type
{
	OnFrontConnected_md,
	OnFrontDisconnected_md,
	OnHeartBeatWarning_md,
	OnRspUserLogin_md,
	OnRspUserLogout_md,
	OnRspError,
	OnRtnDepthMarketData,

	OnFrontConnected_td,
	OnFrontDisconnected_td,
	OnHeartBeatWarning_td,
	OnRspAuthenticate,
	OnRspUserLogin_td,
	OnRspUserLogout_td,

	UnKnown
};

// 预分配固定内存块，覆盖所有 CTP 结构体最大尺寸 (~400 Byte)
// alignas(64) 防止缓存行伪共享 (False Sharing)
struct alignas(64) ctp_task
{
	ctp_task()
		: type(ctp_type::UnKnown)
		, request_id(0)
		, is_last(true)
	{
		std::memset(&data, 0, sizeof(data));
		std::memset(&rsp_info, 0, sizeof(rsp_info));
	}

	ctp_type type;
	//使用 union 节省内存
	union
	{
		int value;
		CThostFtdcRspAuthenticateField auth;
		CThostFtdcRspUserLoginField login;
		CThostFtdcUserLogoutField logout;
		CThostFtdcInstrumentField instrument;
		CThostFtdcDepthMarketDataField quote;

		CThostFtdcInputOrderField input_order;
		CThostFtdcInputOrderActionField order_action;

		CThostFtdcOrderField order;
		CThostFtdcTradeField trade;
		CThostFtdcInvestorPositionField positin;
		CThostFtdcTradingAccountField account;
		CThostFtdcInstrumentMarginRateField margin_rate;
		CThostFtdcInstrumentCommissionRateField commission;

	} data;

	struct
	{
		int errid;
		char err_msg[81];
	} rsp_info;

	int request_id;
	bool is_last;
};
