#pragma once
#include "render_pass.h"

struct SwapChain
{
    VkSwapchainKHR swapChain;
    const Device &device;
    const Window &win;

    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
    VkExtent2D extent;

    std::vector<VkImage> images;
    std::vector<VkImageView> imageViews;
    std::vector<VkFramebuffer> framebuffers;
    uint32_t imageCount;
    RenderPass renderPass;
    SwapChain(
        const Device &_ldev,
        const Window &_win)
        : device(_ldev),
          win(_win),
          surfaceFormat{device.physical.swapChainDetails.chooseSurfaceFormatSrgb()},
          presentMode{device.physical.swapChainDetails.choosePresentModeTriBuffer()},
          extent{device.physical.swapChainDetails.chooseExtent()},
          imageCount{device.physical.swapChainDetails.capabilities.minImageCount + 1},
          renderPass{device}

    {
        if (
            device.physical.swapChainDetails.capabilities.maxImageCount > 0 &&
            imageCount > device.physical.swapChainDetails.capabilities.maxImageCount)
        {
            imageCount = device.physical.swapChainDetails.capabilities.maxImageCount;
        }
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = win.surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        if (device.physical.queueFamilies.firstGraphicsQueueFamily != device.physical.queueFamilies.firstPresentQueueFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = device.physical.queueFamilies.chosen.data();
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;     // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }
        createInfo.preTransform = device.physical.swapChainDetails.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        VkResult result = vkCreateSwapchainKHR(device.device, &createInfo, nullptr, &swapChain);
        debugVkResult(result);

        vkGetSwapchainImagesKHR(device.device, swapChain, &imageCount, nullptr);
        images.resize(imageCount);
        vkGetSwapchainImagesKHR(device.device, swapChain, &imageCount, images.data());
        // CREATE IMAGE VIEWS
        imageViews.resize(imageCount);
        for (size_t i = 0; i < imageCount; i++)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = images[i];

            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = surfaceFormat.format;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            VkResult result = vkCreateImageView(device.device, &createInfo, nullptr, &imageViews[i]);
            debugVkResult(result);
        }
        // CREATE RENDER PASS
        renderPass.create(surfaceFormat.format);
        // CREATE FRAMEBUFFERS
        framebuffers.resize(imageCount);
        for (size_t i = 0; i < imageCount; i++)
        {
            VkImageView attachments[] = {
                imageViews[i]};

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass.renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = extent.width;
            framebufferInfo.height = extent.height;
            framebufferInfo.layers = 1;

            VkResult result = vkCreateFramebuffer(device.device, &framebufferInfo, nullptr, &framebuffers[i]);
            debugVkResult(result);
        }
    }
    ~SwapChain()
    {
        for (size_t i = 0; i < imageCount; i++)
        {
            vkDestroyFramebuffer(device.device, framebuffers[i], nullptr);
            vkDestroyImageView(device.device, imageViews[i], nullptr);
        }
        vkDestroySwapchainKHR(device.device, swapChain, nullptr);
    }
};