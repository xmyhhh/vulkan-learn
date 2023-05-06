

#include <stdexcept>
#include <cstdlib>
#include "triangle-main.h"

void TriangleApplication::run()
{
	this->VulkanApplication::run();
	std::cout << "TriangleApplication run" << std::endl;
	mainLoop();
	cleanup();

}

void TriangleApplication::mainLoop()
{
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
	}
}

void TriangleApplication::cleanup()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}
