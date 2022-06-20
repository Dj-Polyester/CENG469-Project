#pragma once

#include "device.h"
struct Shader
{
    size_t size = 0;
    std::vector<char> code;
    VkShaderModule module_;
    VkPipelineShaderStageCreateInfo stageCreateInfo{};
    const VkShaderStageFlagBits stage;
    Device &device;

    Shader(const Shader &) = delete;
    void operator=(const Shader &) = delete;
    Shader(Shader &&) = delete;
    Shader &operator=(Shader &&) = delete;

    Shader(Device &_device, const std::string &filePath, const VkShaderStageFlagBits &&_stage)
        : device{_device},
          stage(_stage)
    {
        std::ifstream file{filePath, std::ios::ate | std::ios::binary};

        if (!file.is_open())
        {
            ERROR("failed to open file: " + filePath);
        }
        size = static_cast<size_t>(file.tellg());
        file.seekg(0);
        code.reserve(size);
        file.read(code.data(), size);
        file.close();

        VkShaderModuleCreateInfo moduleCreateInfo{};
        moduleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleCreateInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());
        moduleCreateInfo.codeSize = size;

        if (vkCreateShaderModule(device.logical, &moduleCreateInfo, nullptr, &module_) != VK_SUCCESS)
        {
            ERROR("failed to create shader module");
        }

        stageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stageCreateInfo.stage = stage;
        stageCreateInfo.module = module_;
        stageCreateInfo.pName = "main";
        stageCreateInfo.flags = 0;
        stageCreateInfo.pNext = nullptr;
        stageCreateInfo.pSpecializationInfo = nullptr;
    }
    ~Shader()
    {
        vkDestroyShaderModule(device.logical, module_, nullptr);
    }
};
