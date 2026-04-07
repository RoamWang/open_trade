#include "utility.h"
using namespace utility;
#include <sys/stat.h>
#include <limits.h>
#include <unistd.h>

std::string utility::get_executable_path()
{
	char result[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX - 1);
	if (count != -1)
	{
		result[count] = '\0';
		std::string curPath(result);
		return curPath.substr(0, curPath.find_last_of("/") + 1);
	}
	else
	{
		// STLOG_ERROR << "get path error";
		return "";
	}
}

bool utility::check_dir(const std::string& dir)
{
	bool res = false;

	if (access(dir.c_str(), 0) == -1)
	{
		int flag = mkdir(dir.c_str(), 0777);

		if (flag == 0)
		{
			res = true;
		}
		else
		{
			// STLOG_ERROR << "create dir failed : " << dir;
		}
	}

	return res;
}
