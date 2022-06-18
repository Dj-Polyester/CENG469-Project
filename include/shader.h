#pragma once
#include "logical_device.h"

struct Shader
{
    size_t fileSize;
    std::vector<char> file;
    VkShaderModule _module;
    const LogicalDevice &device;
    VkPipelineShaderStageCreateInfo shaderStageCreateInfo{};
    VkShaderModuleCreateInfo shaderModuleCreateInfo{};
    Shader(
        std::string shaderPath,
        const LogicalDevice &_device,
        const VkShaderStageFlagBits &&flagBits) : device(_device)
    {
        std::ifstream file_tmp(shaderPath, std::ios::ate | std::ios::binary);

        if (!file_tmp.is_open())
        {
            ERROR4("Failed to open file!");
        }

        fileSize = (size_t)file_tmp.tellg();
        file.reserve(fileSize);

        file_tmp.seekg(0);
        file_tmp.read(file.data(), fileSize);
        file_tmp.close();
        // create shader module

        shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shaderModuleCreateInfo.codeSize = fileSize;
        shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t *>(file.data());

        VkResult result = vkCreateShaderModule(device.device, &shaderModuleCreateInfo, nullptr, &_module);
        debugVkResult(result);

        shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        shaderStageCreateInfo.stage = flagBits;
        shaderStageCreateInfo.module = _module;
        shaderStageCreateInfo.pName = "main";
        shaderStageCreateInfo.pSpecializationInfo = nullptr;
    }

    ~Shader()
    {
        vkDestroyShaderModule(device.device, _module, nullptr);
    }
};
