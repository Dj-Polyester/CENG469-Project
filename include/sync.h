#pragma once
#include "logical_device.h"

struct Semaphore
{
    VkSemaphore semaphore;
    VkSemaphoreCreateInfo createInfo{};
    const Device &device;
    Semaphore(const Device &_device) : device(_device)
    {
        createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        VkResult result = vkCreateSemaphore(device.device, &createInfo, nullptr, &semaphore);
    }
    ~Semaphore()
    {
        vkDestroySemaphore(device.device, semaphore, nullptr);
    }
};
struct Fence
{
    VkFence fence;
    VkFenceCreateInfo createInfo{};
    const Device &device;
    Fence(const Device &_device) : device(_device)
    {
        createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        VkResult result = vkCreateFence(device.device, &createInfo, nullptr, &fence);
    }
    ~Fence()
    {
        vkDestroyFence(device.device, fence, nullptr);
    }
};