#pragma once

#include "instance.h"
#include <cstring>

struct Window
{
    // window vars
    GLFWwindow *window;
    uint32_t width, height;
    VkSurfaceKHR surface;
    const Instance &instance;
    Window(uint32_t w, uint32_t h, const Instance &_instance)
        : width(w),
          height(h),
          instance(_instance)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(width, height, instance.name.c_str(), nullptr, nullptr);

        VkResult result = glfwCreateWindowSurface(instance.instance, window, nullptr, &surface);
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
    ~Window()
    {
        vkDestroySurfaceKHR(instance.instance, surface, nullptr);
        glfwDestroyWindow(window);
    }
};
