#pragma once
#include "../common/VulkanApplication.h"
class VertexApplication :public VulkanApplication {
public :
	void run();
private:
	const std::vector<Vertex> vertices = {
	{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
	{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
	{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
	};

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	void drawFrame();

	void mainLoop();

	void createGraphicsPipeline();

	void createVertexBuffer();

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void cleanup();
};