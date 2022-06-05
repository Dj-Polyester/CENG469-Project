#pragma once

#include "debug.h"
struct Extension
{
    // available extensions
    std::vector<VkExtensionProperties> extProps;
    // number of available extensions
    uint32_t extCount = 0;
    // layer vars
    std::vector<VkLayerProperties> layerProps;
    uint32_t layerCount = 0;
    // glfw vars
    uint32_t glfwExtCount = 0;
    std::vector<const char *> glfwExtensions;
    // other
    VkResult result;
    Extension()
    {
        
        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
        extProps.resize(extCount);
        result = vkEnumerateInstanceExtensionProperties(nullptr, &extCount, extProps.data());
        debugVkResult(result);
        
        debugVkExtensions(extProps, extCount);
        
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        layerProps.resize(layerCount);
        result = vkEnumerateInstanceLayerProperties(&layerCount, layerProps.data());
        debugVkResult(result);
        
        debugVkLayers(layerProps, layerCount);
        
        const char **glfwExts = glfwGetRequiredInstanceExtensions(&glfwExtCount);
        glfwExtensions.insert(glfwExtensions.begin(), glfwExts, glfwExts + glfwExtCount);

        glfwRequireDebugUtils();
        
        debugGlfwExtensions(glfwExtensions, glfwExtCount);
    }

    void glfwRequire(const char *extName)
    {
        glfwExtensions.push_back(extName);
        ++glfwExtCount;
    }
};
