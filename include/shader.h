#pragma once

#include "includes.h"
struct Shader
{
    size_t size = 0;
    std::vector<char> code;
    VkShaderModule module_;

    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    Shader(const std::string &filePath)
    {
        std::ifstream file{filePath, std::ios::ate | std::ios::binary};

        if (!file.is_open())
        {
            std::runtime_error("Failed to open file: " + filePath);
        }
        size = static_cast<size_t>(file.tellg());
        file.seekg(0);
        code.reserve(size);
        file.read(code.data(), size);
        file.close();
    }
};
