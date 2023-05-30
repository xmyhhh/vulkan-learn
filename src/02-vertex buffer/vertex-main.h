#pragma once
#include "../common/VulkanApplication.h"
class VertexApplication :public VulkanApplication {
public :
	void run();
private:
	const std::vector<Vertex2D> vertices = {
		{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
		{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
		{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
		{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
	};
	//It is possible to use either uint16_t or uint32_t, VK_INDEX_TYPE_UINT16 and VK_INDEX_TYPE_UINT32 for index blind
	const std::vector<uint16_t> indices = {
	0, 1, 2, 2, 3, 0
	};

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;
	VkCommandBuffer commandBuffer;
	void drawFrame();

	void mainLoop();

	void createGraphicsPipeline();

	void createVertexBuffer();

	void createIndexBuffer();

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void createCommandBuffer();

	void cleanup();

	void createSyncObjects();
};