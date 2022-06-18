#pragma once
#include "debug.h"

struct Rasterizer
{
    VkPipelineRasterizationStateCreateInfo stateCreateInfo{};
    Rasterizer()
    {

        stateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        stateCreateInfo.depthClampEnable = VK_FALSE;
        stateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
        stateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
        stateCreateInfo.lineWidth = 1.0f;

        stateCreateInfo.depthBiasEnable = VK_FALSE;
        stateCreateInfo.depthBiasConstantFactor = 0.0f; // Optional
        stateCreateInfo.depthBiasClamp = 0.0f;          // Optional
        stateCreateInfo.depthBiasSlopeFactor = 0.0f;    // Optional
    }
};
