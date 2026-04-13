#include "config.h"

config::config()
{

}

config::~config()
{

}

config& config::instance()
{
	static config _config;
	return _config;
}
