#pragma once

#include "debug.h"

struct Window
{
    // window vars
    GLFWwindow *window;
    uint32_t width, height;
    VkSurfaceKHR surface;
    const std::string name;

    Window(const Window &) = delete;
    void operator=(const Window &) = delete;
    Window(Window &&) = delete;
    Window &operator=(Window &&) = delete;

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
    VkExtent2D getExtent() { return {width, height}; }
    void createWindowSurface(VkInstance &instance, VkSurfaceKHR &surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
        {
            ERROR("failed to create window surface");
        }
    }
    ~Window()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};
