#pragma once

#include "debug.h"

struct Instance
{
    std::string name;
    uint32_t requiredExtensionCount;
    std::vector<const char *> requiredExtensions;
    uint32_t availableExtensionCount;
    std::vector<VkExtensionProperties> availableExtensions;
    uint32_t availableLayerCount;
    std::vector<VkLayerProperties> availableLayers;
#ifdef VALIDATION_ENABLED
    DebugMessenger *debugMessenger;
#endif
    VkInstance vkobject;

    Instance(const std::string &name);
    ~Instance();
    bool checkValidationLayerSupport();
    void getRequiredExtensions();
    void hasRequiredInstanceExtensions();
};
