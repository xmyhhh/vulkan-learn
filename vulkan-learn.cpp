#include <iostream>
#include "src/01-triangle/triangle-main.h"
#include "src/02-vertex buffer/vertex-main.h"
int main()
{
	std::cout << "Hello World!\n";

	//TriangleApplication pApp;
	VertexApplication pApp;
	try {
		pApp.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return 0;
}
