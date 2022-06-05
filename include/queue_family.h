#include "physical_device.h"
struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value();
    }
};

struct QueueFamilyManager
{
    std::vector<VkQueueFamilyProperties> queueFamilies;
    uint32_t queueFamilyCount = 0;
    VkPhysicalDevice device;
    QueueFamilyManager(const VkPhysicalDevice &_device)
    {
        device = _device;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        queueFamilies.resize(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    }
    QueueFamilyIndices &findGraphicsQueueFamilies()
    {
        QueueFamilyIndices indices;
        int i = 0;
        for (const auto &queueFamily : queueFamilies)
        {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphicsFamily = i;
                break;
            }

            i++;
        }
        return indices;
    }
    bool isDeviceSuitable()
    {
        QueueFamilyIndices indices = findGraphicsQueueFamilies();
        return indices.graphicsFamily.has_value();
    }
};
