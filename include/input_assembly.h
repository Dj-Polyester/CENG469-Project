#pragma once
#include "debug.h"

struct InputAssembly
{
    VkPipelineInputAssemblyStateCreateInfo stateCreateInfo{};
    InputAssembly()
    {
        stateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        stateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        stateCreateInfo.primitiveRestartEnable = VK_FALSE;
    }
};
