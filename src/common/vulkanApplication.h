#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>

class VulkanApplication {
public:
	GLFWwindow* window;
	VkInstance instance;
	virtual void run();

private:
	uint32_t WIDTH = 800;
	uint32_t HEIGHT = 600;

	void initVulkan();

	void initWindow();

	void createInstance();

};
