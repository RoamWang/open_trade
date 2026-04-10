#include "md_spi.h"
#include <vector>
#include "utility.h"
#include "string_ptr.h"
#include "ThostFtdcMdApi.h"
#include "user_info.h"

md_spi::md_spi(optimized_queue* queue)
	: api_(nullptr)
	, user_info_(nullptr)
	, queue_(queue)
{

}

md_spi::~md_spi()
{

}

void md_spi::init()
{
	std::string mdConDir = utility::get_executable_path() + "data";
	utility::check_dir(mdConDir);
	mdConDir += "//MD_" + user_info_->brokerid_ + "_" + user_info_->investorid_;
	utility::check_dir(mdConDir);

	CThostFtdcMdApi* pUserApi = CThostFtdcMdApi::CreateFtdcMdApi(mdConDir.c_str(), false, false);
	pUserApi->RegisterSpi(this);

	for (auto& item : user_info_->md_fronts_)
	{
		string_ptr ptr(std::string("tcp://") + item);
		pUserApi->RegisterFront(ptr.get_ptr());

		//STLOG_DEBUG << "register market front = " << fronts[i];
	}
	pUserApi->Init();
}

void md_spi::free_api()
{
	if (api_)
	{
		api_->RegisterSpi(nullptr);
		api_->Release();
	}
}

void md_spi::OnFrontConnected()
{
	ctp_task task;
	task.type = ctp_type::OnFrontConnected_md;
	queue_->push(task);
}

void md_spi::OnFrontDisconnected(int nReason)
{
	ctp_task task;
	task.type = ctp_type::OnFrontDisconnected_md;
	task.data.value = nReason;
	queue_->push(task);
}

void md_spi::OnHeartBeatWarning(int nTimeLapse)
{
	ctp_task task;
	task.type = ctp_type::OnHeartBeatWarning_md;
	task.data.value = nTimeLapse;
	queue_->push(task);
}

void md_spi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	ctp_task task;
	task.type = ctp_type::OnRspUserLogin_md;
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

void md_spi::OnRspUserLogout(CThostFtdcUserLogoutField* pUserLogout, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	ctp_task task;
	task.type = ctp_type::OnRspUserLogout_md;
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

void md_spi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
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

void md_spi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{

}

void md_spi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{

}

void md_spi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	if (!pDepthMarketData)
	{
		return;
	}

	ctp_task task;
	task.type = ctp_type::OnRtnDepthMarketData;
	task.data.quote = *pDepthMarketData;
	if (!queue_->push(task))
	{
		//STLOG_ERROR << "can't push to queue";
	}
}
