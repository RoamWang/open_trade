#pragma once

class user_info
{
public:
	user_info();
	~user_info();

	void init_test();

public:
	std::string brokerid_;
	std::string investorid_;
	std::string password_;
	std::string net_name_;
	std::vector<std::string> td_fronts_;
	std::vector<std::string> md_fronts_;
	std::string authcode_;
	std::string appid_;

	int sessionid_;
	int frontid_;
	uint64_t max_orderref_;
};
