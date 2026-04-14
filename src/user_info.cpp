#include "user_info.h"
#include "log.h"

user_info::user_info()
	: sessionid_(0)
	, frontid_(0)
	, max_orderref_(1)
{

}

user_info::~user_info()
{

}

void user_info::init_test()
{
	STLOG_DEBUG << "init_test user info";
	brokerid_ = "9999";
	investorid_ = "000678";
	password_ = "Wang@cai1234!";
	td_fronts_.push_back("182.254.243.31:30001");
	md_fronts_.push_back("182.254.243.31:30011");

	authcode_ = "0000000000000000";
	appid_ = "SHINNY_Q7V2_2.93";
}

int user_info::generate_reqid()
{
	static int reqid = 1000;
	return reqid++;
}
