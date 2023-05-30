#pragma once
#include "../common/VulkanApplication.h"



class MeshApplication :public VulkanApplication {
	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

public:
	void run();
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;
	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;
	std::vector<void*> uniformBuffersMapped;

	std::vector<VkCommandBuffer> commandBuffers;
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;

	VkImageView textureImageView;
	VkSampler textureSampler;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	uint32_t currentFrame = 0;
private:

	const std::string MODEL_PATH = "src/06-mesh/viking_room.obj";
	const std::string TEXTURE_PATH = "src/06-mesh/viking_room.png";

	std::vector<Vertex3D> vertices;
	std::vector<uint32_t> indices;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;

	VkImage textureImage;
	VkDeviceMemory textureImageMemory;

	void loadModel();

	void drawFrame();

	void mainLoop();

	void createGraphicsPipeline();

	void createVertexBuffer();

	void createIndexBuffer();

	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

	void createDescriptorSetLayout();

	void createUniformBuffers();

	void updateUniformBuffer(uint32_t currentImage);

	void createCommandBuffer();

	void createSyncObjects();

	void createDescriptorPool();

	void createDescriptorSets();

	void createTextureImage();

	void createTextureImageView();

	void createTextureSampler();

	void createDepthResources();

	void createRenderPass();

	void createFramebuffers();

	void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

	VkFormat findDepthFormat();

	void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	void cleanup();
};