#pragma once
#include "pipeline.h"

struct Command
{
    VkCommandPool pool;
    VkCommandBuffer buffer;
    VkCommandBufferAllocateInfo bufferAllocInfo{};
    VkCommandBufferBeginInfo bufferBeginInfo{};

    const Pipeline &pipeline;

    VkCommandPoolCreateInfo poolCreateInfo{};
    VkRenderPassBeginInfo renderPassInfo{};

    Command(const Pipeline &_pipeline) : pipeline(_pipeline)
    {

        poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolCreateInfo.queueFamilyIndex = pipeline.swapChain.device.physical.queueFamilies.firstGraphicsQueueFamily.value();

        VkResult result = vkCreateCommandPool(pipeline.swapChain.device.device, &poolCreateInfo, nullptr, &pool);
        debugVkResult(result);

        bufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        bufferAllocInfo.commandPool = pool;
        bufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        bufferAllocInfo.commandBufferCount = 1;

        result = vkAllocateCommandBuffers(pipeline.swapChain.device.device, &bufferAllocInfo, &buffer);
        debugVkResult(result);
    }
    void record(uint32_t imageIndex)
    {
        vkResetCommandBuffer(buffer, 0);

        bufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        bufferBeginInfo.flags = 0;                  // Optional
        bufferBeginInfo.pInheritanceInfo = nullptr; // Optional

        VkResult result = vkBeginCommandBuffer(buffer, &bufferBeginInfo);
        debugVkResult(result);

        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = pipeline.swapChain.renderPass.renderPass;
        renderPassInfo.framebuffer = pipeline.swapChain.framebuffers[imageIndex];

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.pipeline);
        vkCmdDraw(buffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(buffer);
        result = vkEndCommandBuffer(buffer);
        debugVkResult(result);
    }
    ~Command()
    {
        vkDestroyCommandPool(pipeline.swapChain.device.device, pool, nullptr);
    }
};
