#include "command.h"
#include "sync.h"

void drawFrame(
    Command &command,
    const Semaphore &imageAvailable,
    const Semaphore &renderFinished,
    const Fence &inFlight)
{
    const VkSwapchainKHR &swapChain = command.pipeline.swapChain.swapChain;
    const VkDevice &device = command.pipeline.swapChain.device.device;
    const VkQueue &graphicsQueue = command.pipeline.swapChain.device.graphicsQueue;
    const VkQueue &presentQueue = command.pipeline.swapChain.device.presentQueue;
    VkRenderPassCreateInfo &renderPassCreateInfo = const_cast<VkRenderPassCreateInfo &>(command.pipeline.swapChain.renderPass.createInfo);

    vkWaitForFences(device, 1, &inFlight.fence, VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &inFlight.fence);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(
        device,
        swapChain,
        UINT64_MAX,
        imageAvailable.semaphore,
        VK_NULL_HANDLE,
        &imageIndex);
    debugVkResult(result);
    command.record(imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailable.semaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &command.buffer;

    VkSemaphore signalSemaphores[] = {renderFinished.semaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    result = vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlight.fence);
    debugVkResult(result);

    // dependency

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    result = vkQueuePresentKHR(presentQueue, &presentInfo);
    debugVkResult(result);
}
int main()
{
    try
    {
        // Use when using GLFW
        glfwInit();
        // SETUP
        App app("testProj");
        Extension ext;
        Instance instance(ext, app, "vulcano");
        Window win(800, 600, instance);
        DeviceManager devman(win, instance);
        Device device = devman.bestDeviceOnScore();
        SwapChain swapChain(device, win);
        // GRAPHICS PIPELINE
        Pipeline pipeline(swapChain);
        // COMMAND
        Command command(pipeline);
        // SYNC OBJECTS
        Semaphore imageAvailable(device);
        Semaphore renderFinished(device);
        Fence inFlight(device);

        while (!glfwWindowShouldClose(win.window))
        {
            glfwPollEvents();
            drawFrame(
                command,
                imageAvailable,
                renderFinished,
                inFlight);
        }
        vkDeviceWaitIdle(device.device);
        glfwTerminate();
    }
    catch (const std::exception &e)
    {
        ERROR4(e.what());
    }

    return 0;
}