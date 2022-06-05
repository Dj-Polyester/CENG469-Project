#define VERBOSE_ENABLED
#include "window.h"
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

int main()
{

    try
    {
        // Use when using GLFW
        glfwInit();


        Window win(800, 600, "vulcano");
        /*Extension ext;
        Instance instance(win.name, ext);
        Surface surface(instance.instance, win.window);
        PhysicalDevice physicalDevice(instance.instance, surface.surface);
        LogicalDevice logicalDevice(instance.instance, physicalDevice.physicalDevice, surface.surface);
        SwapChain swapChain(physicalDevice.physicalDevice, instance.instance, surface.surface, logicalDevice.device, win.window);
        ImageViews imageViews(logicalDevice.device, swapChain.swapChainImages, swapChain.swapChainImageFormat, swapChain.swapChainExtent);
        RenderPass renderPass(logicalDevice.device, swapChain.swapChainImageFormat);
        GraphicsPipeline graphicsPipeline(logicalDevice.device, renderPass.renderPass, swapChain.swapChainExtent);
        FrameBuffers frameBuffers(logicalDevice.device, renderPass.renderPass, swapChain.swapChainExtent, imageViews.swapChainImageViews);
        CommandPool commandPool(physicalDevice.physicalDevice, logicalDevice.device, surface.surface);
        CommandBuffer commandBuffer(logicalDevice.device, commandPool.commandPool, renderPass.renderPass, frameBuffers.swapChainFramebuffers, swapChain.swapChainExtent, graphicsPipeline.graphicsPipeline);
        */

        win.run();

        glfwTerminate();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}