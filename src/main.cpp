#include <iostream>
#include <string>
#include <vector>
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

	printf("--- FORCING PTR_CENTER INIT ---\n");
	ptr_center::instance();
	ptr_center::instance().init();
	printf("--- PTR_CENTER INIT DONE ---\n");

	optimized_queue queue;
	main_dispatcher dispatcher(queue);
	dispatcher.init();
	dispatcher.run();

	return 0;
}
