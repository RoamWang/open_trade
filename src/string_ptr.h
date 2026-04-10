#pragma once
#include <string>
#include <cstring>

struct string_ptr
{
	string_ptr()
		: m_ptr(nullptr)
	{

	}
	string_ptr(const std::string& str)
	{
		int len = (int)str.length();
		m_ptr = new char[len + 1];
		memset(m_ptr, 0, len + 1);
		std::memcpy(m_ptr, str.c_str(), len);
		m_ptr[len] = '\0';
	}
	~string_ptr()
	{
		if (m_ptr)
		{
			delete[]m_ptr;
			m_ptr = 0;
		}
	}

	char* get_ptr()
	{
		return m_ptr;
	}

private:
	char* m_ptr;
};
