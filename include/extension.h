#pragma once

#include "debug.h"

struct Extension
{
    // available extensions
    std::vector<VkExtensionProperties> exts;
    // number of available extensions
    uint32_t extCount = 0;
    // available layers
    std::vector<VkLayerProperties> layers;
    // number of available layers
    uint32_t layerCount = 0;
    // required extensions by glfw
    std::vector<const char *> glfwExts;
    // number of required extensions by glfw
    uint32_t glfwExtCount = 0;
    Extension()
    {

        vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
        exts.resize(extCount);
        VkResult result = vkEnumerateInstanceExtensionProperties(nullptr, &extCount, exts.data());
        debugVkResult(result);

        debugVkInstanceExtensions((*this));

        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        layers.resize(layerCount);
        result = vkEnumerateInstanceLayerProperties(&layerCount, layers.data());
        debugVkResult(result);

        debugVkLayers((*this));

        const char **glfwExts_tmp = glfwGetRequiredInstanceExtensions(&glfwExtCount);
        glfwExts.insert(glfwExts.begin(), glfwExts_tmp, glfwExts_tmp + glfwExtCount);

        glfwRequireDebugUtils((*this));

        debugVkInstanceExtensions((*this));
    }

    void glfwRequireExt(const char *extName)
    {
        glfwExts.push_back(extName);
        ++glfwExtCount;
    }
    void glfwRequireExts(const std::vector<const char *> &extNames)
    {
        glfwExts.insert(glfwExts.end(), extNames.begin(), extNames.end());
        glfwExtCount += extNames.size();
    }

    void queryLayers()
    {
        DEBUG2("Available layers (layerCount)", layerCount);
        for (size_t i = 0; i < layerCount; ++i)
        {
            DEBUG(i);
            DEBUG2("layerName", layers[i].layerName);
            DEBUG2("specVersion", layers[i].specVersion);
            DEBUG2("implementationVersion", layers[i].implementationVersion);
            DEBUG2("description", layers[i].description);
        }
    }
    void queryExtensions()
    {
        DEBUG2("Available instance extensions:", extCount);
        for (size_t i = 0; i < extCount; ++i)
        {
            DEBUG(i);
            DEBUG2("extensionName", exts[i].extensionName);
            DEBUG2("specVersion", exts[i].specVersion);
        }
    }
    void queryGlfwExtensions()
    {
        DEBUG2("Required extensions glfw (glfwExtCount)", glfwExtCount);
        for (size_t i = 0; i < glfwExtCount; ++i)
        {
            DEBUG2(i, glfwExts[i]);
        }
    }
};
