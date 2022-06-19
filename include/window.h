#pragma once

#include "includes.h"

struct Window
{
    // window vars
    GLFWwindow *window;
    uint32_t width, height;
    VkSurfaceKHR surface;
    const std::string name;

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    Window(uint32_t w, uint32_t h, const std::string &_name)
        : width(w),
          height(h),
          name(_name)
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    }
    void createWindowSurface(VkInstance &instance, VkSurfaceKHR &surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
        {
            std::runtime_error("Familed to create window surface");
        }
    }
    ~Window()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};
