#include <iostream>
#include <string>
#include <vector>
#include "config.h"
#include "ptr_center.h"
#include "spsc_queue.h"
#include "main_dispatcher.h"
#include "log.h"

int main(int argc, char* argv[])
{
	light::Logger::Instance().InitConsoleLog();
	light::Logger::Instance().InitPersistLog(false, true);
	light::Logger::Instance().Filter(static_cast<SeverityLevel>(1));

	STLOG_DEBUG << "starting";

	config::instance();
	ptr_center::instance();

	optimized_queue queue;
	main_dispatcher dispatcher(queue);
	dispatcher.init();
	dispatcher.run();

	return 0;
}
