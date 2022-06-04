#pragma once

#include "extension.h"
#include <cstring>

struct Window
{
    // win vars
    GLFWwindow *window;
    uint32_t width, height;
    std::string name;
    // instance vars
    VkInstance instance;
    VkResult result;
    VkDebugUtilsMessengerEXT debugMessenger;
    // constructors
    Window(uint32_t w, uint32_t h, std::string n) : width(w), height(h), name(n)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

        if (ext::extCount == UINT32_MAX)
        {
            vkEnumerateInstanceExtensionProperties(nullptr, &ext::extCount, nullptr);
            ext::extProps.resize(ext::extCount);
            result = vkEnumerateInstanceExtensionProperties(nullptr, &ext::extCount, ext::extProps.data());
            debugVkResult(result);
        }
        if (ext::layerCount == UINT32_MAX)
        {
            vkEnumerateInstanceLayerProperties(&ext::layerCount, nullptr);
            ext::layerProps.resize(ext::layerCount);
            result = vkEnumerateInstanceLayerProperties(&ext::layerCount, ext::layerProps.data());
            debugVkResult(result);
        }
        debugVkExtensions(ext::extProps, ext::extCount);
        createVkInstance();
        debugVkLayers(ext::layerProps, ext::layerCount);
        setupDebugMessenger(instance, debugMessenger);
    }
    ~Window()
    {
        destroyDebugMessenger(instance, debugMessenger);
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
    }
    Window(const Window &win) = delete;
    Window &operator=(const Window &win) = delete;

    void createVkInstance()
    {
        layerSupport(ext::layerProps);

        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = name.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo{};

        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        if (ext::glfwExtCount == UINT32_MAX)
        {
            const char **glfwExts = glfwGetRequiredInstanceExtensions(&ext::glfwExtCount);
            ext::glfwExtensions.insert(ext::glfwExtensions.begin(), glfwExts, glfwExts + ext::glfwExtCount);

            glfwRequireVal(ext::glfwExtensions, ext::glfwExtCount);
        }

        debugGlfwExtensions(ext::glfwExtensions, ext::glfwExtCount);

        createInfo.enabledExtensionCount = static_cast<uint32_t>(ext::glfwExtensions.size());
        createInfo.ppEnabledExtensionNames = ext::glfwExtensions.data();

        std::cout << ext::glfwExtensions[0];
        createInfoLayerSet(createInfo);

        result = vkCreateInstance(&createInfo, nullptr, &instance);
        debugVkResult(result);
    }

    void run()
    {
        try
        {
            while (!glfwWindowShouldClose(window))
            {
                glfwPollEvents();
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
};
