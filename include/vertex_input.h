#pragma once
#include "debug.h"

struct VertexInput
{
    VkPipelineVertexInputStateCreateInfo stateCreateInfo{};
    VertexInput()
    {
        stateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        stateCreateInfo.vertexBindingDescriptionCount = 0;
        stateCreateInfo.pVertexBindingDescriptions = nullptr; // Optional
        stateCreateInfo.vertexAttributeDescriptionCount = 0;
        stateCreateInfo.pVertexAttributeDescriptions = nullptr; // Optional
    }
};