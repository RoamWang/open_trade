#include <iostream>
#include <string>
#include <vector>
#include "config.h"
#include "ptr_center.h"
#include "spsc_queue.h"
#include "main_dispatcher.h"

int main(int argc, char* argv[])
{
	config::instance();
	ptr_center::instance();

	optimized_queue queue;
	main_dispatcher dispatcher(queue);
	dispatcher.init();
	dispatcher.run();

	return 0;
}
