#pragma once
#include "spsc_queue.h"
#include "ThostFtdcUserApiStruct.h"

class md_spi;
class trade_spi;
class user_info;
class main_dispatcher
{
public:
	main_dispatcher(optimized_queue& queue);
	~main_dispatcher();

	void init();
	void run();

private:
	// 返回处理的任务数量，用于主循环动态调整策略
	int process_batch();
	void handle_task(const ctp_task& task);
	void handle_error(const ctp_task& task);

	void on_front_connected_md();
	void on_front_disconnected_md(int reason);
	void on_login_md(const CThostFtdcRspUserLoginField& field);

	void on_rtn_depth_marketdata(const CThostFtdcDepthMarketDataField& pDepthMarketData);

	void on_front_connected_td();
	void on_front_disconnected_td(int reason);
	void on_login_td(const CThostFtdcRspUserLoginField& field);

private:
	void req_subscribe_quote();
private:
	optimized_queue& queue_;

	md_spi* md_spi_;
	trade_spi* trade_spi_;

	user_info* user_info_;
};
