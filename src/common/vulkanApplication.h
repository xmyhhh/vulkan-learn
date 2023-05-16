#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

class VulkanApplication {
public:
	GLFWwindow* window;
	VkInstance instance;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;
	VkQueue graphicsQueue;


#ifdef NDEBUG
	static const bool enableValidationLayers = false;
#else
	static const bool enableValidationLayers = true;
#endif

	virtual void run();
	virtual void cleanup();
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

	bool checkValidationLayerSupport();
};
