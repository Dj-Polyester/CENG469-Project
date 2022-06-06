#include "physical_device.h"

struct LogicalDevice
{
    VkDevice device{};
    VkPhysicalDeviceFeatures features{};
    VkQueue graphicsQueue{};
    LogicalDevice(const PhysicalDevice &pdev)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = pdev.firstGraphicsQueueFamily.value();
        queueCreateInfo.queueCount = 1;
        //[0.0,1.0]
        float queuePriority = 1.0f;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;

        createInfo.pEnabledFeatures = &features;
        // enableValLayer(createInfo);

        VkResult result = vkCreateDevice(pdev.device, &createInfo, nullptr, &device);
        debugVkResult(result);
        // first queue of firstGraphicsQueueFamily
        vkGetDeviceQueue(device, pdev.firstGraphicsQueueFamily.value(), 0, &graphicsQueue);
    }
    ~LogicalDevice()
    {
        vkDestroyDevice(device, nullptr);
    }
};
