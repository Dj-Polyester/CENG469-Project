#pragma once

#include "debug.h"

struct App
{
    VkApplicationInfo info{};
    App(const std::string &name)
    {
        info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        info.pApplicationName = name.c_str();
        info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        info.pEngineName = "No Engine";
        info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        info.apiVersion = VK_API_VERSION_1_0;
    }
};
