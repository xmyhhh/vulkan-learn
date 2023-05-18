#pragma once
#include "../common/VulkanApplication.h"
class TriangleApplication :public VulkanApplication {
public:

	void run();


private:
	void drawFrame();

	void mainLoop();

	void createGraphicsPipeline();

	
};