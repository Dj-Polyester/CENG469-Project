#pragma once

#include "extension.h"
#include <cstring>

struct Instance
{
    // instance vars
    VkInstance instance;
    VkResult result;
    VkDebugUtilsMessengerEXT debugMessenger;
    Extension ext;
    Instance(std::string appname, Extension &_ext)
    {
        ext = _ext;
        debugLayerSupport(ext.layers);

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

        createInfo.enabledExtensionCount = static_cast<uint32_t>(ext.glfwExts.size());
        createInfo.ppEnabledExtensionNames = ext.glfwExts.data();

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
