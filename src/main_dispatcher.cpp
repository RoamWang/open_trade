#include "main_dispatcher.h"
#include "md_spi.h"
#include "trade_spi.h"
#include "user_info.h"
#include "ptr_center.h"
#include <thread>

main_dispatcher::main_dispatcher(optimized_queue& queue)
	: queue_(queue)
	, md_spi_(nullptr)
	, trade_spi_(nullptr)
{
	user_info_ = ptr_center::instance().get_user_info();
}

main_dispatcher::~main_dispatcher()
{
	if (md_spi_)
	{
		md_spi_->free_api();
		delete md_spi_;
		md_spi_ = nullptr;
	}

	if (trade_spi_)
	{
		trade_spi_->free_api();
		delete trade_spi_;
		trade_spi_ = nullptr;
	}
}

void main_dispatcher::init()
{
	md_spi_ = new md_spi(&queue_);
	md_spi_->init();

	trade_spi_ = new trade_spi(&queue_);
	trade_spi_->init();
}

void main_dispatcher::run()
{
	while (true)
	{
		int count = process_batch();
		if (count > 0)
		{
			continue;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
	}
}

int main_dispatcher::process_batch()
{
	ctp_task task;
	int processed = 0;
	// 单次最多处理 512 条，防止主线程卡死
	const int MAX_BACTH = 512;

	while (processed < MAX_BACTH && queue_.pop(task))
	{
		if (task.rsp_info.errid != 0)
		{
			handle_error(task);
		}
		else
		{
			handle_task(task);
		}
		++processed;
	}

	return processed;
}

void main_dispatcher::handle_task(const ctp_task& task)
{
	switch (task.type)
	{
	case ctp_type::OnFrontConnected_md:
	{
		on_front_connected_md();
	}
	break;
	case ctp_type::OnFrontDisconnected_md:
	{
		on_front_disconnected_md(task.data.value);
	}
	break;
	case ctp_type::OnRtnDepthMarketData:
	{
		on_rtn_depth_marketdata(task.data.quote);
	}
	break;
	default:
		break;
	}
}

void main_dispatcher::handle_error(const ctp_task& task)
{

}

void main_dispatcher::on_front_connected_md()
{
	md_spi_->req_login(user_info_->generate_reqid());
}

void main_dispatcher::on_front_disconnected_md(int reason)
{
	//STLOG_DEBUG << "market disconnected, reason = " << reason;
}

void main_dispatcher::on_login_md(const CThostFtdcRspUserLoginField& field)
{
	req_subscribe_quote();
}

void main_dispatcher::on_rtn_depth_marketdata(const CThostFtdcDepthMarketDataField& pDepthMarketData)
{

}

void main_dispatcher::on_front_connected_td()
{
	trade_spi_->req_login(user_info_->generate_reqid());
}

void main_dispatcher::on_front_disconnected_td(int reason)
{
	//STLOG_DEBUG << "trade disconnected, reason = " << reason;
}

void main_dispatcher::on_login_td(const CThostFtdcRspUserLoginField& field)
{

}

void main_dispatcher::req_subscribe_quote()
{

}
