#include "md_spi.h"
#include <vector>
#include "utility.h"
#include "string_ptr.h"
#include "user_info.h"
#include "ptr_center.h"
#include "log.h"

md_spi::md_spi(optimized_queue* queue)
	: api_(nullptr)
	, user_info_(nullptr)
	, queue_(queue)
{
	user_info_ = ptr_center::instance().get_user_info();
}

md_spi::~md_spi()
{

}

void md_spi::init()
{
	STLOG_DEBUG << "init md";
	std::string mdConDir = utility::get_executable_path() + "data";
	utility::check_dir(mdConDir);
	mdConDir += "//MD_" + user_info_->brokerid_ + "_" + user_info_->investorid_;
	utility::check_dir(mdConDir);

	api_ = CThostFtdcMdApi::CreateFtdcMdApi(mdConDir.c_str(), false, false);
	api_->RegisterSpi(this);

	for (auto& item : user_info_->md_fronts_)
	{
		string_ptr ptr(std::string("tcp://") + item);
		STLOG_DEBUG << "register market front = " << ptr.get_ptr();
		api_->RegisterFront(ptr.get_ptr());
	}
	api_->Init();
}

void md_spi::free_api()
{
	if (api_)
	{
		api_->RegisterSpi(nullptr);
		api_->Release();
	}
}

int md_spi::req_login(int reqid)
{
	STLOG_INFO << "begin";
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	if (!user_info_)
	{
		STLOG_ERROR << "user_info_ is null";
	}
	STLOG_ERROR << "111";
	strncpy(req.BrokerID, user_info_->brokerid_.c_str(), sizeof(req.BrokerID));
	STLOG_ERROR << "222";
	strncpy(req.UserID, user_info_->investorid_.c_str(), sizeof(req.UserID));
	STLOG_ERROR << "333";
	strncpy(req.Password, user_info_->password_.c_str(), sizeof(req.Password));
	STLOG_ERROR << "444";
	if (!api_)
	{
		STLOG_ERROR << "api_ is null";
	}
	int res = api_->ReqUserLogin(&req, reqid);

	STLOG_INFO << "ReqUserLogin res = " << res;
	return res;
}

//////////////////////////////////////////////////////////////////////////
void md_spi::OnFrontConnected()
{
	STLOG_DEBUG << "OnFrontConnected";
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
