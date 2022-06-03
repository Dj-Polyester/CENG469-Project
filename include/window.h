#pragma once

#include "extension.h"
#include <cstring>

struct Window
{

    GLFWwindow *window;
    uint32_t width, height;
    std::string name;

    Window(uint32_t w, uint32_t h, std::string n) : width(w), height(h), name(n)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);
    }
    ~Window()
    {
        glfwDestroyWindow(window);
    }
    Window(const Window &win) = delete;
    Window &operator=(const Window &win) = delete;

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
