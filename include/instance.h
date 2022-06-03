#pragma once

#include "extension.h"
#include <cstring>

struct Instance
{
    // instance vars
    VkInstance instance;
    VkResult result;
    VkDebugUtilsMessengerEXT debugMessenger;

    Instance(std::string appname, Extension &ext)
    {
        debugLayerSupport(ext.layerProps);

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = appname.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(ext.glfwExtensions.size());
        createInfo.ppEnabledExtensionNames = ext.glfwExtensions.data();

        enableValLayer(createInfo);

        result = vkCreateInstance(&createInfo, nullptr, &instance);
        debugVkResult(result);
        setupDebugMessenger(instance, debugMessenger);
    }
    ~Instance()
    {
        destroyDebugMessenger(instance, debugMessenger);
        vkDestroyInstance(instance, nullptr);
    }
};
