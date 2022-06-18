#pragma once
#include "logical_device.h"
struct RenderPass
{
    VkRenderPass renderPass;
    VkRenderPassCreateInfo createInfo{};
    VkAttachmentDescription colorAttachment{};
    VkAttachmentReference colorAttachmentRef{};
    VkSubpassDescription subpass{};

    const LogicalDevice &device;
    RenderPass(const LogicalDevice &_device)
        : device(_device)
    {
    }
    void create(VkFormat format)
    {
        colorAttachment.format = format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; // swap chain

        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        createInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        createInfo.attachmentCount = 1;
        createInfo.pAttachments = &colorAttachment;
        createInfo.subpassCount = 1;
        createInfo.pSubpasses = &subpass;

        VkResult result = vkCreateRenderPass(device.device, &createInfo, nullptr, &renderPass);
        debugVkResult(result);
    }
    ~RenderPass()
    {
        vkDestroyRenderPass(device.device, renderPass, nullptr);
    }
};
