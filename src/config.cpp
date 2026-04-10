#include "config.h"
#include "user_info.h"

config::config()
	: user_info_(nullptr)
{
	user_info_ = new user_info();
	user_info_->init_test();
}

config::~config()
{

}
