#pragma once
#include "physical_device.h"

struct LogicalDevice
{
    VkDevice device{};
    VkPhysicalDeviceFeatures features{};
    VkQueue graphicsQueue{};
    VkQueue presentQueue{};
    const PhysicalDevice &physical;
    uint32_t uniqueQueueFamilyCount;
    LogicalDevice(const PhysicalDevice &_physical)
        : physical(_physical)
    {
        float queuePriority = 1.0f;

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        uniqueQueueFamilyCount = physical.queueFamilies.chosenUnique.size();
        queueCreateInfos.reserve(uniqueQueueFamilyCount);
        for (uint32_t queueFamily : physical.queueFamilies.chosenUnique)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = uniqueQueueFamilyCount;

        createInfo.pEnabledFeatures = &features;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExts.size());
        createInfo.ppEnabledExtensionNames = deviceExts.data();

        enableValLayer(createInfo);

        VkResult result = vkCreateDevice(_physical.device, &createInfo, nullptr, &device);
        debugVkResult(result);
        // first queue of firstGraphicsQueueFamily
        vkGetDeviceQueue(device, _physical.queueFamilies.firstGraphicsQueueFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, _physical.queueFamilies.firstPresentQueueFamily.value(), 0, &presentQueue);
    }
    ~LogicalDevice()
    {
        vkDestroyDevice(device, nullptr);
    }
};
