#include "ptr_center.h"
#include "user_info.h"
#include "log.h"
#include <iostream>

ptr_center::ptr_center()
	: user_info_(nullptr)
{

}

ptr_center::~ptr_center()
{

}

ptr_center& ptr_center::instance()
{
	static ptr_center _instance;
	return _instance;
}

void ptr_center::init()
{
	std::cout << "init" << std::endl;
	user_info_ = new user_info();
	user_info_->init_test();
}

user_info* ptr_center::get_user_info() const
{
	return user_info_;
}

