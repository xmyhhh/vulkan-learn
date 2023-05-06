#pragma once
#include "../common/VulkanApplication.h"
class TriangleApplication :public VulkanApplication {
public:

	void run();
	void cleanup();

private:
	void mainLoop();

	

};