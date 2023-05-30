#include <iostream>
#include "src/01-triangle/triangle-main.h"
#include "src/02-vertex buffer/vertex-main.h"
#include "src/03-uniform buffer/UBO-main.h"
#include "src/04-texture/texture-main.h"
#include "src/05-Depth buffering/Depth-main.h"
#include "src/06-mesh/mesh-main.h"
#include "src/07-mipmap/mipmap-main.h"
#include "src/08-Multisampling/multisampling-main.h"
int main()
{
	std::cout << "Hello World!\n";

	//TriangleApplication app;
	//VertexApplication app;
	//UBOApplication app;
	//TextureApplication app;
	DepthApplication app;
	//MeshApplication app;
	//MipmapApplication app;
	//MultisamplingApplication app;
	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}
