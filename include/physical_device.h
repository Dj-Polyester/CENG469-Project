#pragma once
#include "swap_chain_details.h"
#include "long.h"

const std::vector<const char *> deviceExts = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

struct PhysicalDevice
{
    // available extensions
    std::vector<VkExtensionProperties> exts;
    // number of available extensions
    uint32_t extCount = 0;

    VkPhysicalDeviceProperties properties{};
    VkPhysicalDeviceFeatures features{};
    VkPhysicalDevice device = VK_NULL_HANDLE;

    SwapChainDetails swapChainDetails;
    // chosen queue families;
    QueueFamilies queueFamilies;

    const Window &win;

    PhysicalDevice(
        VkPhysicalDevice &_device,
        const Window &_win)
        : win(_win),
          queueFamilies{_win},
          swapChainDetails{_win}
    {
        // get properties and features
        device = _device;
        vkGetPhysicalDeviceProperties(device, &properties);
        vkGetPhysicalDeviceFeatures(device, &features);
        // get queue families
        queueFamilies.create(device);
        // get extensions
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, nullptr);
        exts.resize(extCount);
        VkResult result = vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, exts.data());
        debugVkResult(result);

        debugVkDeviceExtensions((*this));
    }
    PhysicalDevice(const PhysicalDevice &&pdev)
        : win(pdev.win),
          device(pdev.device),
          properties(pdev.properties),
          features(pdev.features),
          queueFamilies(std::move(pdev.queueFamilies)),
          swapChainDetails(std::move(pdev.swapChainDetails)),
          exts(std::move(pdev.exts))
    {
    }
    PhysicalDevice(const PhysicalDevice &pdev)
        : win(pdev.win),
          device(pdev.device),
          properties(pdev.properties),
          features(pdev.features),
          queueFamilies(std::move(pdev.queueFamilies)),
          swapChainDetails(std::move(pdev.swapChainDetails)),
          exts(std::move(pdev.exts))
    {
    }
    void queryExtensions()
    {
        DEBUG2("Available device extensions:", extCount);
        for (size_t i = 0; i < extCount; ++i)
        {
            DEBUG(i);
            DEBUG2("extensionName", exts[i].extensionName);
            DEBUG2("specVersion", exts[i].specVersion);
        }
    }
    bool allDeviceExtensionsSupported()
    {
        std::set<std::string> requiredExts(deviceExts.begin(), deviceExts.end());
        for (const auto &ext : exts)
        {
            requiredExts.erase(ext.extensionName);
        }
        return requiredExts.empty();
    }
    bool isSuitable()
    {
        queueFamilies.getFirstSuitableQueueFamilies();
        bool deviceExtensionsSupported = allDeviceExtensionsSupported();
        if (deviceExtensionsSupported)
        {
            swapChainDetails.querySwapChainSupport(device);
        }
        return queueFamilies.complete() && deviceExtensionsSupported && swapChainDetails.compatible();
    }
    uint32_t score() const
    {
        uint32_t score = 0;

        // Discrete GPUs have a significant performance advantage
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }

        // Maximum possible size of textures affects graphics quality
        score += properties.limits.maxImageDimension2D;

        // Application can't function without geometry shaders
        if (!features.geometryShader)
        {
            return 0;
        }

        return score;
    }
    void query()
    {
        debugPhysicalDeviceFeatures(features);
        debugPhysicalDeviceProperties(properties);
    }
};
