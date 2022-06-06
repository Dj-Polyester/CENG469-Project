#pragma once

#include "extension.h"
#include <cstring>
#include "app.h"
struct Instance
{
    // instance vars
    VkInstance instance{};
    VkDebugUtilsMessengerEXT debugMessenger{};
    const Extension &ext;
    const App &app;
    const std::string name;
    Instance(
        const Extension &_ext,
        const App &_app,
        const std::string &_name)
        : ext(_ext),
          app(_app),
          name(_name)
    {
        debugLayerSupport(ext.layers);
        VkInstanceCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &app.info;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(ext.glfwExts.size());
        createInfo.ppEnabledExtensionNames = ext.glfwExts.data();

        enableValLayer(createInfo);

        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
        debugVkResult(result);

        setupDebugMessenger(instance, debugMessenger);
    }
    ~Instance()
    {
        destroyDebugMessenger(instance, debugMessenger);
        vkDestroyInstance(instance, nullptr);
    }
};
