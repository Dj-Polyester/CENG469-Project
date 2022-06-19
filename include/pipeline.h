#pragma once
#include "shader.h"
#include "device.h"

struct PipelineConfigInfo
{
};

struct Pipeline
{

    Pipeline(const Pipeline &) = delete;
    Pipeline &operator=(const Pipeline &) = delete;

    MyEngineDevice &device;
    VkPipeline graphicsPipeline;
    Pipeline(
        MyEngineDevice &_device,
        const std::string &vertPath,
        const std::string &fragPath,
        const PipelineConfigInfo &configInfo)
        : device(_device)
    {
        Shader vertShader{vertPath};
        Shader fragShader{fragPath};
    }
};
