#pragma once
#include "debug.h"
struct ColorBlending
{
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    VkPipelineColorBlendStateCreateInfo stateCreateInfo{};
    ColorBlending()
    {
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;             // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;             // Optional
    }
    void alphaBlending()
    {
        colorBlendAttachment.blendEnable = VK_TRUE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    }
    void attach()
    {
        stateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        stateCreateInfo.logicOpEnable = VK_FALSE;
        stateCreateInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
        stateCreateInfo.attachmentCount = 1;
        stateCreateInfo.pAttachments = &colorBlendAttachment;
        stateCreateInfo.blendConstants[0] = 0.0f; // Optional
        stateCreateInfo.blendConstants[1] = 0.0f; // Optional
        stateCreateInfo.blendConstants[2] = 0.0f; // Optional
        stateCreateInfo.blendConstants[3] = 0.0f; // Optional
    }
};
