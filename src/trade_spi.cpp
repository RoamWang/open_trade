#include "trade_spi.h"
#include <vector>
#include "utility.h"
#include "string_ptr.h"
#include "user_info.h"
#include "ptr_center.h"
#include "log.h"

trade_spi::trade_spi(optimized_queue* queue)
	: api_(nullptr)
	, user_info_(nullptr)
	, queue_(queue)
{
	user_info_ = ptr_center::instance().get_user_info();
}

trade_spi::~trade_spi()
{

}

void trade_spi::init()
{
	std::string tdConDir = utility::get_executable_path() + "data";
	utility::check_dir(tdConDir);
	tdConDir += "//TD_" + user_info_->brokerid_ + "_" + user_info_->investorid_;
	utility::check_dir(tdConDir);

	api_ = CThostFtdcTraderApi::CreateFtdcTraderApi(tdConDir.c_str());
	api_->RegisterSpi(this);
	api_->SubscribePrivateTopic(THOST_TERT_RESTART);
	api_->SubscribePublicTopic(THOST_TERT_RESTART);
	for (auto& item : user_info_->md_fronts_)
	{
		string_ptr ptr(std::string("tcp://") + item);
		api_->RegisterFront(ptr.get_ptr());
		STLOG_DEBUG << "trade front = " << item;
	}
	api_->Init();
}

void trade_spi::free_api()
{
	if (api_)
	{
		api_->RegisterSpi(nullptr);
		api_->Release();
	}
}

int trade_spi::ret_authenticate(int reqid)
{
	CThostFtdcReqAuthenticateField req;
	memset(&req, 0, sizeof(req));
	strncpy(req.BrokerID, user_info_->brokerid_.c_str(), sizeof(req.BrokerID));
	strncpy(req.UserID, user_info_->investorid_.c_str(), sizeof(req.UserID));
	strncpy(req.AuthCode, "0000000000000000", sizeof(req.AuthCode));
	strncpy(req.AppID, "simnow_client_test", sizeof(req.AppID));

	int res = api_->ReqAuthenticate(&req, reqid);
	return res;
}

int trade_spi::req_login(int reqid)
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strncpy(req.BrokerID, user_info_->brokerid_.c_str(), sizeof(req.BrokerID));
	strncpy(req.UserID, user_info_->investorid_.c_str(), sizeof(req.UserID));
	strncpy(req.Password, user_info_->password_.c_str(), sizeof(req.Password));
	int res = api_->ReqUserLogin(&req, reqid);
	return res;
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
