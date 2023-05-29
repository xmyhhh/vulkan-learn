#include <iostream>
#include "src/01-triangle/triangle-main.h"
#include "src/02-vertex buffer/vertex-main.h"
#include "src/03-uniform buffer/UBO-main.h"
int main()
{
	std::cout << "Hello World!\n";

	//TriangleApplication app;
	//VertexApplication app;
	UBOApplication app;
	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}
