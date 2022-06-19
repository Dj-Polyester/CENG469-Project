#pragma once
#include "swap_chain.h"
#include "shader.h"
#include "vertex_input.h"
#include "input_assembly.h"
#include "viewport.h"
#include "rasterizer.h"
#include "multisampler.h"
#include "color_blending.h"

struct Pipeline
{
    VkGraphicsPipelineCreateInfo createInfo{};
    VkPipelineLayout layout;
    VkPipeline pipeline;
    VkPipelineLayoutCreateInfo layoutInfo{};
    const SwapChain &swapChain;
    Pipeline(const SwapChain &_swapChain)
        : swapChain(_swapChain)
    {
        Shader vertshader("/home/polyester/Desktop/Programming/CENG469-Project/shaders/spv/tri_vert.spv", swapChain.device, VK_SHADER_STAGE_VERTEX_BIT);
        Shader fragshader("/home/polyester/Desktop/Programming/CENG469-Project/shaders/spv/tri_frag.spv", swapChain.device, VK_SHADER_STAGE_FRAGMENT_BIT);
        std::vector<VkPipelineShaderStageCreateInfo> shaderStages = {
            vertshader.shaderStageCreateInfo,
            fragshader.shaderStageCreateInfo,
        };
        VertexInput vertexInput;
        InputAssembly inputAssembly;
        Viewport viewport(swapChain);
        Rasterizer rasterizer;
        MultiSampler multisampler;
        // depth stencil testing
        // EMPTY
        // color blending
        ColorBlending colorBlending;
        colorBlending.alphaBlending();
        colorBlending.attach();

        // std::vector<VkDynamicState> dynamicStates = {
        //     VK_DYNAMIC_STATE_VIEWPORT,
        //     VK_DYNAMIC_STATE_LINE_WIDTH};

        // VkPipelineDynamicStateCreateInfo dynamicState{};
        // dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        // dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
        // dynamicState.pDynamicStates = dynamicStates.data();

        layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        layoutInfo.setLayoutCount = 0;            // Optional
        layoutInfo.pSetLayouts = nullptr;         // Optional
        layoutInfo.pushConstantRangeCount = 0;    // Optional
        layoutInfo.pPushConstantRanges = nullptr; // Optional

        VkResult result = vkCreatePipelineLayout(swapChain.device.device, &layoutInfo, nullptr, &layout);
        debugVkResult(result);

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages.data();

        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages.data();

        pipelineInfo.pVertexInputState = &vertexInput.stateCreateInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly.stateCreateInfo;
        pipelineInfo.pViewportState = &viewport.stateCreateInfo;
        pipelineInfo.pRasterizationState = &rasterizer.stateCreateInfo;
        pipelineInfo.pMultisampleState = &multisampler.stateCreateInfo;
        pipelineInfo.pDepthStencilState = nullptr; // Optional
        pipelineInfo.pColorBlendState = &colorBlending.stateCreateInfo;
        pipelineInfo.pDynamicState = nullptr; // Optional

        pipelineInfo.layout = layout;

        pipelineInfo.renderPass = swapChain.renderPass.renderPass;
        pipelineInfo.subpass = 0;

        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1;              // Optional

        result = vkCreateGraphicsPipelines(swapChain.device.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline);
        debugVkResult(result);
    }
    ~Pipeline()
    {
        vkDestroyPipeline(swapChain.device.device, pipeline, nullptr);
        vkDestroyPipelineLayout(swapChain.device.device, layout, nullptr);
    }
};
