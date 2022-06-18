#pragma once
#include "debug.h"

struct MultiSampler
{
    VkPipelineMultisampleStateCreateInfo stateCreateInfo{};
    MultiSampler()
    {

        stateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        stateCreateInfo.sampleShadingEnable = VK_FALSE;
        stateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        stateCreateInfo.minSampleShading = 1.0f;          // Optional
        stateCreateInfo.pSampleMask = nullptr;            // Optional
        stateCreateInfo.alphaToCoverageEnable = VK_FALSE; // Optional
        stateCreateInfo.alphaToOneEnable = VK_FALSE;      // Optional
    }
};
