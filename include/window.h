#pragma once

#include "extension.h"
#include <cstring>
#include "instance.h"
#include "physicalDevice.h"
#include "logicalDevice.h"
#include "surface.h"
#include "swapChain.h"
#include "imageViews.h"
#include "renderPass.h"
#include "graphicsPipeline.h"
#include "frameBuffers.h"
#include "commandPool.h"
#include "commandBuffer.h"
#include "syncObjects.h"

struct Window
{

    GLFWwindow *window;
    uint32_t width, height;
    std::string name;

    Extension* ext;
    Instance* instance;
    Surface* surface;
    PhysicalDevice* physicalDevice;
    LogicalDevice* logicalDevice;
    SwapChain* swapChain;
    ImageViews* imageViews;
    RenderPass* renderPass;
    GraphicsPipeline* graphicsPipeline;
    FrameBuffers* frameBuffers;
    CommandPool* commandPool;
    CommandBuffer* commandBuffer;
    SyncObjects* syncObjects;

    Window(uint32_t w, uint32_t h, std::string n) : width(w), height(h), name(n)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        window = glfwCreateWindow(width, height, name.c_str(), nullptr, nullptr);

        ext = new Extension;
        instance = new Instance(name, *ext);
        surface = new Surface(instance->instance, window);
        physicalDevice = new PhysicalDevice(instance->instance, surface->surface);
        logicalDevice = new LogicalDevice(instance->instance, physicalDevice->physicalDevice, surface->surface);
        swapChain = new SwapChain(physicalDevice->physicalDevice, instance->instance, surface->surface, logicalDevice->device, window);
        imageViews = new ImageViews(logicalDevice->device, swapChain->swapChainImages, swapChain->swapChainImageFormat, swapChain->swapChainExtent);
        renderPass = new RenderPass(logicalDevice->device, swapChain->swapChainImageFormat);
        graphicsPipeline = new GraphicsPipeline(logicalDevice->device, renderPass->renderPass, swapChain->swapChainExtent);
        frameBuffers = new FrameBuffers(logicalDevice->device, renderPass->renderPass, swapChain->swapChainExtent, imageViews->swapChainImageViews);
        commandPool = new CommandPool(physicalDevice->physicalDevice, logicalDevice->device, surface->surface);
        commandBuffer = new CommandBuffer(logicalDevice->device, commandPool->commandPool, renderPass->renderPass, frameBuffers->swapChainFramebuffers, swapChain->swapChainExtent, graphicsPipeline->graphicsPipeline);
        syncObjects = new SyncObjects(logicalDevice->device);

    }
    ~Window()
    {
        glfwDestroyWindow(window);
    }
    Window(const Window &win) = delete;
    Window &operator=(const Window &win) = delete;
    
    void drawFrame() {
        vkWaitForFences(logicalDevice->device, 1, &(syncObjects->inFlightFence), VK_TRUE, UINT64_MAX);
        vkResetFences(logicalDevice->device, 1, &(syncObjects->inFlightFence));

        uint32_t imageIndex;
        vkAcquireNextImageKHR(logicalDevice->device, swapChain->swapChain, UINT64_MAX, (syncObjects->imageAvailableSemaphore), VK_NULL_HANDLE, &imageIndex);

        vkResetCommandBuffer(commandBuffer->commandBuffer, /*VkCommandBufferResetFlagBits*/ 0);
        commandBuffer->recordCommandBuffer(commandBuffer->commandBuffer, imageIndex);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = { syncObjects->imageAvailableSemaphore };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &(commandBuffer->commandBuffer);

        VkSemaphore signalSemaphores[] = { syncObjects->renderFinishedSemaphore };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(logicalDevice->graphicsQueue, 1, &submitInfo, (syncObjects->inFlightFence)) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { swapChain->swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;

        presentInfo.pImageIndices = &imageIndex;

        vkQueuePresentKHR(logicalDevice->presentQueue, &presentInfo);
    }

    void run()
    {
        try
        {
            while (!glfwWindowShouldClose(window))
            {
                glfwPollEvents();
                drawFrame();
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }
};
