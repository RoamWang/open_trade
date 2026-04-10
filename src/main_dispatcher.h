#pragma once
#include "spsc_queue.h"
#include "ThostFtdcUserApiStruct.h"

class md_spi;
class trade_spi;
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

	void OnRtnDepthMarketData(const CThostFtdcDepthMarketDataField& pDepthMarketData);

private:
	optimized_queue& queue_;

	md_spi* md_spi_;
	trade_spi* trade_spi_;
};
