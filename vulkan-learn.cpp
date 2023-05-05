#include <iostream>
#include "src/01-triangle/triangle-main.h"
int main()
{
	std::cout << "Hello World!\n";

	TriangleApplication pApp;

	try {
		pApp.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}
