#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <set>
#include <optional>
#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp
#include <fstream>
#include <string>
class VulkanApplication {
public:
	GLFWwindow* window;
	VkInstance instance;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	VkSurfaceKHR surface;

	VkQueue graphicsQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	std::vector<VkImage> swapChainImages;

	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;

	VkCommandPool commandPool;
	VkCommandBuffer commandBuffer;

	VkSemaphore imageAvailableSemaphore;
	VkSemaphore renderFinishedSemaphore;
	VkFence inFlightFence;

	const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};

#ifdef NDEBUG
	static const bool enableValidationLayers = false;
#else
	static const bool enableValidationLayers = true;
#endif

	virtual void run();
	virtual void cleanup();

	static std::vector<char> readFile(const std::string& filename);
	VkShaderModule createShaderModule(const std::vector<char>& code);
	void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);


private:
	const uint32_t WIDTH = 800;
	const uint32_t HEIGHT = 600;

	std::vector<const char*> validationLayers = {
	"VK_LAYER_KHRONOS_validation"
	};


	VkDebugUtilsMessengerEXT debugMessenger;

	void initVulkan();

	void initWindow();

	void createInstance();

	void setupDebugMessenger();

	void pickPhysicalDevice();

	void createLogicalDevice();

	void createSurface();

	bool checkValidationLayerSupport();

	void createSwapChain();

	void createImageViews();

	void createCommandPool();

	void createCommandBuffer();



	void createSyncObjects();

	virtual void createRenderPass();

	virtual void createGraphicsPipeline() = 0;

	virtual void createFramebuffers();
};
