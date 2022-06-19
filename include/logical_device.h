#pragma once
#include "physical_device.h"

struct Device
{
    VkDevice device{};
    VkPhysicalDeviceFeatures features{};
    VkQueue graphicsQueue{};
    VkQueue presentQueue{};
    const PhysicalDevice &physical;
    uint32_t uniqueQueueFamilyCount;
    Device(const PhysicalDevice &_physical)
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
    ~Device()
    {
        vkDestroyDevice(device, nullptr);
    }
};
struct DeviceManager
{
    std::optional<uint32_t> bestDeviceOnScoreIndex;
    uint32_t deviceCount = 0;
    std::vector<PhysicalDevice> devices;

    const Window &win;
    const Instance &instance;
    DeviceManager(
        const Window &_win,
        const Instance &_instance)
        : win(_win),
          instance(_instance)
    {
        vkEnumeratePhysicalDevices(instance.instance, &deviceCount, nullptr);
        if (deviceCount == 0)
        {
            ERROR4("Failed to find devices with Vulkan support");
        }
        std::vector<VkPhysicalDevice> devices_tmp(deviceCount);
        vkEnumeratePhysicalDevices(instance.instance, &deviceCount, devices_tmp.data());
        devices.reserve(deviceCount);
        for (size_t i = 0; i < deviceCount; i++)
        {
            devices.push_back(PhysicalDevice(devices_tmp[i], win));
        }
        debugPhysicalDevices((*this));
    }
    void queryDevices()
    {
        DEBUG2("Available devices (deviceCount)", deviceCount);
        for (size_t i = 0; i < deviceCount; ++i)
        {
            DEBUG(i);
            debugPhysicalDevice(devices[i]);
        }
    }

    PhysicalDevice &bestDeviceOnScore()
    {
        uint32_t val;
        if (bestDeviceOnScoreIndex.has_value())
        {
            return devices[bestDeviceOnScoreIndex.value()];
        }

        // Use an ordered map to automatically sort candidates by increasing score
        std::multimap<uint32_t, uint32_t> candidates;
        uint32_t i = 0;
        for (const auto &device : devices)
        {
            int score = device.score();
            candidates.insert(std::make_pair(score, i));
            ++i;
        }

        // Check if the best candidate is suitable at all
        if (candidates.rbegin()->first > 0 && devices[candidates.rbegin()->second].isSuitable())
        {
            bestDeviceOnScoreIndex = candidates.rbegin()->second;
        }
        else
        {
            ERROR4("Failed to find a suitable GPU!");
        }
        return devices[bestDeviceOnScoreIndex.value()];
    }
};