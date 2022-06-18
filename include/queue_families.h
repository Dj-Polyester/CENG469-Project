#pragma once
#include "window.h"

struct QueueFamilies
{
    std::optional<uint32_t> firstGraphicsQueueFamily;
    std::optional<bool> graphicsSupport;
    std::optional<uint32_t> firstPresentQueueFamily;
    std::optional<bool> presentSupport;
    std::vector<VkQueueFamilyProperties> list;
    uint32_t count = 0;
    std::set<uint32_t> chosenUnique;
    std::vector<uint32_t> chosen;
    VkPhysicalDevice device;
    const Window &win;
    QueueFamilies(const Window &_win)
        : win(_win)
    {
    }
    void create(const VkPhysicalDevice &_device)
    {
        device = _device;
        // get queue families
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, nullptr);
        list.resize(count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &count, list.data());
    }
    inline bool complete()
    {
        return firstGraphicsQueueFamily.has_value() &&
               firstPresentQueueFamily.has_value();
    }
    void getFirstSuitableQueueFamilies()
    {
        for (size_t i = 0; i < count; i++)
        {
            VkBool32 presentSupport = false;
            VkResult result = vkGetPhysicalDeviceSurfaceSupportKHR(device, i, win.surface, &presentSupport);
            debugVkResult(result);
            if (list[i].queueFlags & VK_QUEUE_GRAPHICS_BIT & presentSupport)
            {
                firstGraphicsQueueFamily = i;
                firstPresentQueueFamily = i;
                break;
            }
        }
        chosenUnique = {
            firstGraphicsQueueFamily.value(),
            firstPresentQueueFamily.value()};
        chosen = {
            firstGraphicsQueueFamily.value(),
            firstPresentQueueFamily.value()};
    }
};
