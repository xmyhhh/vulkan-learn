#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <set>
#include <optional>
#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp
#include <fstream>
#include <string>
#include <array>

struct Vertex {
	glm::vec2 pos;
	glm::vec3 color;

	//对于所有顶点数据的表述
	static VkVertexInputBindingDescription getBindingDescription() {
		VkVertexInputBindingDescription bindingDescription{
			//binding参数指定绑定数组中绑定的索引
			.binding = 0,
			//步幅参数指定从一个条目到下一个条目的字节数
			.stride = sizeof(Vertex),
			//VK_VERTEX_INPUT_RATE_VERTEX: Move to the next data entry after each vertex
			//VK_VERTEX_INPUT_RATE_INSTANCE : Move to the next data entry after each instance
			.inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
		};
		return bindingDescription;
	}

	//对于单个顶点数据的每个属性表述
	static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
		//An attribute description struct describes how to extract a vertex attribute from a chunk of vertex data originating from a binding description
		//We have two attributes, position and color, so we need two attribute description structs
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

		{
			//The binding parameter tells Vulkan from which binding the per-vertex data comes
			attributeDescriptions[0].binding = 0;
			//The location parameter references the location directive of the input in the vertex shader.
			attributeDescriptions[0].location = 0;
			//The input in the vertex shader with location 0 is the position, which has two 32-bit float components.
			/*float: VK_FORMAT_R32_SFLOAT
			vec2 : VK_FORMAT_R32G32_SFLOAT
			vec3 : VK_FORMAT_R32G32B32_SFLOAT
			vec4 : VK_FORMAT_R32G32B32A32_SFLOAT*/
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			/*should also match the type of the shader input.See the following examples :
			ivec2: VK_FORMAT_R32G32_SINT, a 2 - component vector of 32 - bit signed integers
			uvec4 : VK_FORMAT_R32G32B32A32_UINT, a 4 - component vector of 32 - bit unsigned integers
			double : VK_FORMAT_R64_SFLOAT, a double - precision(64 - bit) float*/

			//The format parameter implicitly defines the byte size of attribute data
			//The offset parameter specifies the number of bytes since the start of the per - vertex data to read from.
			attributeDescriptions[0].offset = offsetof(Vertex, pos);
		}

		{
			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);
		}

		return attributeDescriptions;
	}
};

struct SwapChainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices {
	//支持图形命令的队列族和支持表达的队列族可能不重叠
	std::optional<uint32_t> graphicsFamily;  //supporting drawing commands
	std::optional<uint32_t> presentFamily;   //supporting presentation commands

	bool isComplete() {
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

class VulkanApplication {
public:
	GLFWwindow* window;
	VkInstance instance;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	VkSurfaceKHR surface;

	VkQueue graphicsQueue;
	VkQueue presentQueue;


	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;
	VkSwapchainKHR swapChain;
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

	SwapChainSupportDetails swapChainSupport;
	QueueFamilyIndices indices;

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

	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);


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

	virtual void createVertexBuffer();
	};
