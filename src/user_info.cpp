#include "user_info.h"

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
	brokerid_ = "9999";
	investorid_ = "000678";
	password_ = "Wang@cai1234!";
	td_fronts_.push_back("180.168.146.187:10201");
	md_fronts_.push_back("180.168.146.187:10211");

	authcode_ = "0000000000000000";
	appid_ = "SHINNY_Q7V2_2.93";
}
