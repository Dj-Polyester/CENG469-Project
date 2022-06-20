#pragma once

#include "pipeline.h"
#include "model.h"

struct App
{

    Window win;
    Device device;
    SwapChain swapChain;
    std::vector<VkCommandBuffer> commandBuffers;
    std::unique_ptr<Pipeline> pipeline;
    std::unique_ptr<Model> model;

    App(const App &) = delete;
    void operator=(const App &) = delete;
    App(App &&) = delete;
    App &operator=(App &&) = delete;

    App()
        : win{WIDTH, HEIGHT, "hello"},
          device{win},
          swapChain{device, win.getExtent()}
    {
        pipeline = std::make_unique<Pipeline>("./shaders/spv/tri.vert.spv",
                                              "./shaders/spv/tri.frag.spv",
                                              swapChain);

        const std::vector<Vertex> vertices = {
            {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};

        model = std::make_unique<Model>(
            device, vertices);
        commandBuffers.resize(swapChain.imageCount);
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = swapChain.device.commandPool;
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(swapChain.device.logical, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
        {
            ERROR("failed to allocate command buffers");
        }
        for (size_t i = 0; i < commandBuffers.size(); i++)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
            {
                ERROR("failed to begin recording command buffer");
            }
            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = swapChain.renderPass;
            renderPassInfo.framebuffer = swapChain.frameBuffers[i];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapChain.extent;

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {.1, .1, .1, 1};
            clearValues[1].depthStencil = {1, 0};

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            pipeline->bind(commandBuffers[i]);
            model->bind(commandBuffers[i]);
            model->draw(commandBuffers[i]);
            vkCmdEndRenderPass(commandBuffers[i]);
            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
            {
                ERROR("failed to record command buffer");
            }
        }
    }
    void drawFrame()
    {
        uint32_t imageIndex;
        auto result = swapChain.acquireNextImage(&imageIndex);
        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            ERROR("failed to acquire swap chain image");
        }
        result = swapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
        if (result != VK_SUCCESS)
        {
            ERROR("failed to present swap chain image");
        }
    }
    ~App()
    {
    }
    void run()
    {
        while (!glfwWindowShouldClose(win.window))
        {
            glfwPollEvents();
            drawFrame();
        }
        vkDeviceWaitIdle(device.logical);
    }
};
