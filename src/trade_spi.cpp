#include "trade_spi.h"
#include "user_info.h"

trade_spi::trade_spi(optimized_queue* queue)
	: api_(nullptr)
	, user_info_(nullptr)
	, queue_(queue)
{

}

trade_spi::~trade_spi()
{

}

void trade_spi::init()
{

}

void trade_spi::free_api()
{
	if (api_)
	{
		api_->RegisterSpi(nullptr);
		api_->Release();
	}
}

void trade_spi::OnFrontConnected()
{
	ctp_task task;
	task.type = ctp_type::OnFrontConnected_td;
	queue_->push(task);
}

void trade_spi::OnFrontDisconnected(int nReason)
{
	ctp_task task;
	task.type = ctp_type::OnFrontDisconnected_td;
	task.data.value = nReason;
	queue_->push(task);
}

void trade_spi::OnHeartBeatWarning(int nTimeLapse)
{
	ctp_task task;
	task.type = ctp_type::OnHeartBeatWarning_td;
	task.data.value = nTimeLapse;
	queue_->push(task);
}

void trade_spi::OnRspAuthenticate(CThostFtdcRspAuthenticateField* pRspAuthenticateField, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	ctp_task task;
	task.type = ctp_type::OnRspAuthenticate;
	if (pRspAuthenticateField)
	{
		task.data.auth = *pRspAuthenticateField;
	}
	if (pRspInfo)
	{
		task.rsp_info.errid = pRspInfo->ErrorID;
		std::strncpy(task.rsp_info.err_msg, pRspInfo->ErrorMsg, sizeof(task.rsp_info.err_msg));
	}
	task.request_id = nRequestID;
	task.is_last = bIsLast;

	queue_->push(task);
}

void trade_spi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	ctp_task task;
	task.type = ctp_type::OnRspUserLogin_td;
	if (pRspUserLogin)
	{
		task.data.login = *pRspUserLogin;
	}
	if (pRspInfo)
	{
		task.rsp_info.errid = pRspInfo->ErrorID;
		std::strncpy(task.rsp_info.err_msg, pRspInfo->ErrorMsg, sizeof(task.rsp_info.err_msg));
	}
	task.request_id = nRequestID;
	task.is_last = bIsLast;

	queue_->push(task);
}

void trade_spi::OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	ctp_task task;
	task.type = ctp_type::OnRspUserLogout_td;
	if (pUserLogout)
	{
		task.data.logout = *pUserLogout;
	}
	if (pRspInfo)
	{
		task.rsp_info.errid = pRspInfo->ErrorID;
		std::strncpy(task.rsp_info.err_msg, pRspInfo->ErrorMsg, sizeof(task.rsp_info.err_msg));
	}
	task.request_id = nRequestID;
	task.is_last = bIsLast;

	queue_->push(task);
}

void trade_spi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	ctp_task task;
	task.type = ctp_type::OnRspError;
	if (pRspInfo)
	{
		task.rsp_info.errid = pRspInfo->ErrorID;
		std::strncpy(task.rsp_info.err_msg, pRspInfo->ErrorMsg, sizeof(task.rsp_info.err_msg));
	}
	task.request_id = nRequestID;
	task.is_last = bIsLast;

	queue_->push(task);
}
