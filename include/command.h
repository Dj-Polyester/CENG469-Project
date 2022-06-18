#pragma once
#include "logical_device.h"

struct Command
{
    VkCommandPool pool;
    VkCommandBuffer commandBuffer;
    VkCommandBufferAllocateInfo bufferAllocInfo{};

    const PhysicalDevice &pdev;
    const LogicalDevice &ldev;
    VkCommandPoolCreateInfo poolCreateInfo{};
    Command(const PhysicalDevice &_pdev, const LogicalDevice &_ldev)
        : pdev(_pdev),
          ldev(_ldev)
    {

        poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolCreateInfo.queueFamilyIndex = pdev.queueFamilies.firstGraphicsQueueFamily.value();

        VkResult result = vkCreateCommandPool(ldev.device, &poolCreateInfo, nullptr, &pool);
        debugVkResult(result);

        bufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        bufferAllocInfo.commandPool = pool;
        bufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        bufferAllocInfo.commandBufferCount = 1;

        result = vkAllocateCommandBuffers(ldev.device, &bufferAllocInfo, &commandBuffer);
        debugVkResult(result);
    }
    ~Command()
    {
        vkDestroyCommandPool(ldev.device, pool, nullptr);
    }
};
