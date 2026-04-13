#pragma once

class user_info;
class ptr_center
{
public:
	static ptr_center& instance();

	user_info* get_user_info() const;
private:
	user_info* user_info_;

private:
	ptr_center();
	~ptr_center();
};

