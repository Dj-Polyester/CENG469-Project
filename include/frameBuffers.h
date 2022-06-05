#pragma once

#include "extension.h"
#include <cstring>
#include <fstream> 



struct FrameBuffers
{

	VkDevice device;

	VkRenderPass renderPass;

	VkExtent2D swapChainExtent;

	std::vector<VkFramebuffer> swapChainFramebuffers;

	std::vector<VkImageView> swapChainImageViews;

	FrameBuffers(VkDevice _device, VkRenderPass _renderPass, VkExtent2D _swapChainExtent, std::vector<VkImageView> _swapChainImageViews)
	{
		device = _device;
		renderPass = _renderPass;
		swapChainImageViews = _swapChainImageViews;
		swapChainExtent = _swapChainExtent;

		swapChainFramebuffers.resize(swapChainImageViews.size());

		for (size_t i = 0; i < swapChainImageViews.size(); i++) {
			VkImageView attachments[] = {
				swapChainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo{};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderPass;
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = swapChainExtent.width;
			framebufferInfo.height = swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to create framebuffer!");
			}
		}

	}

	~FrameBuffers()
	{
		for (auto framebuffer : swapChainFramebuffers) {
			vkDestroyFramebuffer(device, framebuffer, nullptr);
		}
	}


};