#include <iostream>
#include <string>
#include <vector>
#include "spsc_queue.h"
#include "main_dispatcher.h"

int main(int argc, char* argv[])
{
	optimized_queue queue;
	main_dispatcher dispatcher(queue);
	dispatcher.init();
	dispatcher.run();

	return 0;
}
