#pragma once

#include "instance.h"

struct Window
{
    // window vars
    Instance &instance;
    GLFWwindow *window;
    uint32_t width, height;
    VkSurfaceKHR surface;

    bool frameBufferResizedFlag = false;

    Window(const Window &) = delete;
    void operator=(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(Window &&) = delete;

    Window(uint32_t w, uint32_t h, Instance &instance_)
        : width(w),
          height(h),
          instance(instance_)
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        window = glfwCreateWindow(width, height, instance.name.c_str(), nullptr, nullptr);
        VkResult result = glfwCreateWindowSurface(instance.vkobject, window, nullptr, &surface);
        if (result != VK_SUCCESS)
        {
            ERROR("failed to create window surface");
        }
    }
    VkExtent2D extent() { return {width, height}; }
    ~Window()
    {
        vkDestroySurfaceKHR(instance.vkobject, surface, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    static void frameBufferResized(GLFWwindow *window, int width, int height)
    {
        glfwGetWindowUserPointer(window);
    }
};
