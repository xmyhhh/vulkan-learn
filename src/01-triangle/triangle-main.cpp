

#include <stdexcept>
#include <cstdlib>
#include "triangle-main.h"

void TriangleApplication::run()
{
	VulkanApplication::run();
	std::cout << "TriangleApplication run" << std::endl;
	mainLoop();
	cleanup();

}

void TriangleApplication::recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex) {
	//If the command buffer was already recorded once, 
	//then a call to vkBeginCommandBuffer will implicitly reset it. 
	//It's not possible to append commands to a buffer at a later time. vkBeginCommandBuffer锟斤拷锟斤拷锟斤拷CommandBuffer
	VkCommandBufferBeginInfo beginInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = 0, // Optional
		.pInheritanceInfo = nullptr, // Optional
	};

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	VkRenderPassBeginInfo renderPassInfo{
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		.renderPass = renderPass,
		.framebuffer = swapChainFramebuffers[imageIndex],
		.clearValueCount = 1,
		.pClearValues = &clearColor,
	};
	renderPassInfo.renderArea.offset = { 0, 0 },
		renderPassInfo.renderArea.extent = swapChainExtent,

		//VK_SUBPASS_CONTENTS_INLINE: The render pass commands will be embedded in the primary command buffer itself and no secondary command buffers will be executed.
		//VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS: The render pass commands will be executed from secondary command buffers.
		vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(swapChainExtent.width);
		viewport.height = static_cast<float>(swapChainExtent.height);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = swapChainExtent;
		vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

		vkCmdDraw(commandBuffer, 3, 1, 0, 0);
	}
	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

}


void TriangleApplication::drawFrame()
{
	vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
	//After waiting, we need to manually reset the fence to the unsignaled state
	vkResetFences(device, 1, &inFlightFence);

	uint32_t imageIndex;
	vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
	vkResetCommandBuffer(commandBuffer, 0);
	recordCommandBuffer(commandBuffer, imageIndex);
	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = { imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	//wait on before execution begins and in which stage(s) of the pipeline to wait
	submitInfo.pWaitSemaphores = waitSemaphores;
	//wait with writing colors to the image until it's available
	//That means that theoretically the implementation can already start executing our vertex shader and such while the image is not yet available
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	VkSemaphore signalSemaphores[] = { renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;
	VkSwapchainKHR swapChains[] = { swapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	//submits the request to present an image to the swap chain
	vkQueuePresentKHR(presentQueue, &presentInfo);
}

void TriangleApplication::mainLoop()
{
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		drawFrame();
	}
	//wait for the logical device to finish operations before exiting mainLoop and destroying the window
	vkDeviceWaitIdle(device);
}

void TriangleApplication::createGraphicsPipeline()
{
	auto vertShaderCode = readFile("src/01-triangle/shader/vert.spv");
	auto fragShaderCode = readFile("src/01-triangle/shader/frag.spv");

	VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
	VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

	VkPipelineShaderStageCreateInfo vertShaderStageInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_VERTEX_BIT,
		.module = vertShaderModule,
		.pName = "main",
	};

	VkPipelineShaderStageCreateInfo fragShaderStageInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
		.stage = VK_SHADER_STAGE_FRAGMENT_BIT,
		.module = fragShaderModule,
		.pName = "main",
	};

	VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

	/*VkPipelineVertexInputStateCreateInfo锟结构锟斤拷锟斤拷锟斤拷锟斤拷锟捷革拷锟斤拷锟斤拷锟斤拷色锟斤拷锟侥讹拷锟斤拷锟斤拷锟捷的革拷式锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟街凤拷式锟斤拷锟斤拷锟斤拷锟斤拷一锟姐：
		锟斤拷Bindings锟斤拷锟斤拷锟斤拷之锟斤拷募锟洁，锟皆硷拷锟斤拷锟斤拷锟角帮拷锟斤拷锟姐还锟角帮拷实锟斤拷锟斤拷锟斤拷渭锟绞碉拷锟斤拷锟斤拷锟�
		锟斤拷锟斤拷锟斤拷锟斤拷Attribute descriptions锟斤拷锟斤拷锟捷革拷锟斤拷锟斤拷锟斤拷色锟斤拷锟斤拷锟斤拷锟皆碉拷锟斤拷锟酵ｏ拷锟斤拷锟侥革拷锟襟定硷拷锟斤拷锟斤拷些锟斤拷锟斤拷锟皆硷拷偏锟斤拷锟斤拷
	锟斤拷为锟斤拷锟斤拷直锟斤拷锟节讹拷锟斤拷锟斤拷色锟斤拷锟叫对讹拷锟斤拷锟斤拷锟捷斤拷锟斤拷硬锟斤拷锟诫，锟斤拷锟斤拷锟斤拷锟角斤拷锟斤拷锟剿结构锟斤拷指锟斤拷目前没锟斤拷要锟斤拷锟截的讹拷锟斤拷锟斤拷锟捷★拷锟斤拷锟角斤拷锟节讹拷锟姐缓锟斤拷锟斤拷一锟斤拷锟叫回碉拷锟斤拷一锟姐。*/
	VkPipelineVertexInputStateCreateInfo vertexInputInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
		.vertexBindingDescriptionCount = 0,
		.vertexAttributeDescriptionCount = 0,
	};

	/*VkPipelineInputAssemblyStateCreateInfo锟结构锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟铰ｏ拷
		what kind of geometry will be drawn from the vertices锟斤拷
		if primitive restart should be enabled锟斤拷
	前锟斤拷锟斤拷锟斤拷锟剿筹拷员锟斤拷指锟斤拷锟斤拷锟斤拷锟皆撅拷锟斤拷锟斤拷锟斤拷值锟斤拷
		VK_PRIMITIVE_TOPOLOGY_POINT_LIST: points from vertices
		VK_PRIMITIVE_TOPOLOGY_LINE_LIST: line from every 2 vertices without reuse
		VK_PRIMITIVE_TOPOLOGY_LINE_STRIP: the end vertex of every line is used as start vertex for the next line
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST: triangle from every 3 vertices without reuse
		VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: the second and third vertex of every triangle are used as first two vertices of the next triangle
	*/
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
		.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		.primitiveRestartEnable = VK_FALSE,
	};

	VkPipelineViewportStateCreateInfo viewportState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
		.viewportCount = 1,
		.scissorCount = 1,
	};

	/*锟斤拷栅锟斤拷锟斤拷锟接讹拷锟斤拷锟斤拷色锟斤拷锟叫伙拷取锟缴讹拷锟斤拷锟斤拷锟斤拷募锟斤拷锟斤拷澹拷锟斤拷锟斤拷锟阶拷锟轿拷锟斤拷锟斤拷锟狡拷锟斤拷锟缴拷锟斤拷锟缴拷锟�
	锟斤拷锟斤拷执锟斤拷锟斤拷炔锟斤拷浴锟斤拷娌匡拷蕹锟斤拷图锟斤拷锟斤拷锟皆ｏ拷锟斤拷锟揭匡拷锟斤拷锟斤拷锟斤拷为锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷位锟斤拷锟斤拷锟斤拷缘锟斤拷片锟轿ｏ拷锟竭匡拷锟斤拷染锟斤拷锟斤拷*/
	VkPipelineRasterizationStateCreateInfo rasterizer{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
		//锟斤拷锟絛epthClampEnable锟斤拷锟斤拷锟斤拷为VK_TRUE锟斤拷锟斤拷么锟节斤拷平锟斤拷锟皆镀斤拷锟街拷锟斤拷片锟轿斤拷锟斤拷锟斤拷住锟斤拷锟斤拷锟斤拷锟角讹拷锟斤拷锟斤拷锟角★拷
		.depthClampEnable = VK_FALSE,
		.rasterizerDiscardEnable = VK_FALSE,
		//VK_POLYGON_MODE_FILL: fill the area of the polygon with fragments
		//VK_POLYGON_MODE_LINE : polygon edges are drawn as lines
		//VK_POLYGON_MODE_POINT : polygon vertices are drawn as points
		.polygonMode = VK_POLYGON_MODE_FILL,
		.cullMode = VK_CULL_MODE_BACK_BIT,
		.frontFace = VK_FRONT_FACE_CLOCKWISE,
		.depthBiasEnable = VK_FALSE,
		.lineWidth = 1.0f,
	};

	VkPipelineMultisampleStateCreateInfo multisampling{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
		.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
		.sampleShadingEnable = VK_FALSE,
	};

	//片锟斤拷锟斤拷色锟斤拷锟斤拷锟斤拷锟斤拷色锟斤拷锟斤拷要锟斤拷锟斤拷锟斤拷帧锟斤拷锟斤拷锟斤拷锟斤拷锟窖撅拷锟斤拷锟节碉拷锟斤拷色锟斤拷稀锟�
	//锟斤拷锟斤拷转锟斤拷锟斤拷锟斤拷为锟斤拷色锟斤拷希锟斤拷锟斤拷锟斤拷址锟斤拷锟斤拷锟斤拷锟绞碉拷郑锟�
	//	锟斤拷锟铰撅拷值锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟缴�
	//	使锟斤拷锟斤拷位锟斤拷锟斤拷锟斤拷锟斤拷戮锟街�
	VkPipelineColorBlendAttachmentState colorBlendAttachment{
		.blendEnable = VK_FALSE,
		.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
	};

	VkPipelineColorBlendStateCreateInfo colorBlending{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
		.logicOpEnable = VK_FALSE,
		.logicOp = VK_LOGIC_OP_COPY,
		.attachmentCount = 1,
		.pAttachments = &colorBlendAttachment,
	};

	colorBlending.blendConstants[0] = 0.0f;
	colorBlending.blendConstants[1] = 0.0f;
	colorBlending.blendConstants[2] = 0.0f;
	colorBlending.blendConstants[3] = 0.0f;

	std::vector<VkDynamicState> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_SCISSOR
	};
	VkPipelineDynamicStateCreateInfo dynamicState{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
		.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
		.pDynamicStates = dynamicStates.data(),
	};

	VkPipelineLayoutCreateInfo pipelineLayoutInfo{
		.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.setLayoutCount = 0,
		.pushConstantRangeCount = 0,
	};

	if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo{
		.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
		//referencing the array of VkPipelineShaderStageCreateInfo structs
		.stageCount = 2,
		.pStages = shaderStages,
		//reference all of the structures describing the fixed-function stage.
		.pVertexInputState = &vertexInputInfo,
		.pInputAssemblyState = &inputAssembly,
		.pViewportState = &viewportState,
		.pRasterizationState = &rasterizer,
		.pMultisampleState = &multisampling,
		.pDepthStencilState = nullptr, // Optional
		.pColorBlendState = &colorBlending,
		.pDynamicState = &dynamicState,
		//pipeline layout
		.layout = pipelineLayout,
		.renderPass = renderPass,
		.subpass = 0,
		.basePipelineHandle = VK_NULL_HANDLE,
	};

	if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
		throw std::runtime_error("failed to create graphics pipeline!");
	}
	vkDestroyShaderModule(device, fragShaderModule, nullptr);
	vkDestroyShaderModule(device, vertShaderModule, nullptr);
}

void TriangleApplication::createSyncObjects()
{
	VkSemaphoreCreateInfo semaphoreInfo{
	.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO
	};
	VkFenceCreateInfo fenceInfo{
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT,
	};

	if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
		vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS ||
		vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence) != VK_SUCCESS) {
		throw std::runtime_error("failed to create semaphores!");
	}
}

void TriangleApplication::createCommandBuffer() {
	VkCommandBufferAllocateInfo allocInfo{
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = commandPool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = 1,
	};

	if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}