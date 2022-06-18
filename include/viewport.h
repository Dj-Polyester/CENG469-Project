#pragma once
#include "swap_chain.h"
struct Viewport
{
    VkViewport viewport{};
    VkRect2D scissor{};
    VkPipelineViewportStateCreateInfo stateCreateInfo{};
    Viewport(const SwapChain &swapChain)
    {
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float)swapChain.extent.width;
        viewport.height = (float)swapChain.extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        scissor.offset = {0, 0};
        scissor.extent = swapChain.extent;

        stateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        stateCreateInfo.viewportCount = 1;
        stateCreateInfo.pViewports = &viewport;
        stateCreateInfo.scissorCount = 1;
        stateCreateInfo.pScissors = &scissor;
    }
};
