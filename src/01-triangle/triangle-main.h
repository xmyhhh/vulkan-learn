#pragma once
#include "../common/VulkanApplication.h"
class TriangleApplication :public VulkanApplication {
public:

	void run();


private:
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void drawFrame();

	void mainLoop();

	void createGraphicsPipeline();

	
};