#pragma once

#include "queue_families.h"

struct SwapChainDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    uint32_t formatCount = 0;
    std::vector<VkPresentModeKHR> presentModes;
    uint32_t presentModeCount = 0;

    const Window &win;
    SwapChainDetails(const Window &_win)
        : win(_win)
    {
    }
    inline bool compatible()
    {
        return !formats.empty() && !presentModes.empty();
    }
    void querySwapChainSupport(const VkPhysicalDevice &device)
    {
        VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, win.surface, &capabilities);
        debugVkResult(result);

        result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, win.surface, &formatCount, nullptr);
        debugVkResult(result);
        formats.resize(formatCount);
        result = vkGetPhysicalDeviceSurfaceFormatsKHR(device, win.surface, &formatCount, formats.data());
        debugVkResult(result);

        result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, win.surface, &presentModeCount, nullptr);
        debugVkResult(result);
        presentModes.resize(presentModeCount);
        result = vkGetPhysicalDeviceSurfacePresentModesKHR(device, win.surface, &presentModeCount, presentModes.data());
        debugVkResult(result);
    }
    VkSurfaceFormatKHR chooseSurfaceFormatSrgb() const
    {
        for (const auto &availableFormat : formats)
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
            {
                return availableFormat;
            }
        }
        return formats[0];
    }
    VkPresentModeKHR choosePresentModeTriBuffer() const
    {
        for (const auto &availablePresentMode : presentModes)
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                return availablePresentMode;
            }
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }
    VkExtent2D chooseExtent() const
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            return capabilities.currentExtent;
        }
        else
        {
            int width, height;
            glfwGetFramebufferSize(win.window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)};

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }
};