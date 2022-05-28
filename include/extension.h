#pragma once

#include "debug.h"
namespace ext
{
    // extension vars
    std::vector<VkExtensionProperties> extProps;
    uint32_t extCount = UINT32_MAX;
    // layer vars
    std::vector<VkLayerProperties> layerProps;
    uint32_t layerCount = UINT32_MAX;
    // glfw vars
    uint32_t glfwExtCount = UINT32_MAX;
    std::vector<const char *> glfwExtensions;
}
