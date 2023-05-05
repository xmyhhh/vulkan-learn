#pragma once
#include "../common/common.h"
class TriangleApplication :public VulkanApplication {
public:
	void run();

private:
	void initVulkan();

	void mainLoop();

	void cleanup();
};