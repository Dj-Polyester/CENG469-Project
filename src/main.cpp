#include "pipeline.h"
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
        PhysicalDeviceManager pdevman(win, instance);
        PhysicalDevice pdev = pdevman.bestDeviceOnScore();
        LogicalDevice ldev(pdev);
        SwapChain swapChain(pdev, ldev, win);
        // GRAPHICS PIPELINE
        Pipeline pipeline(ldev, swapChain);
        win.run();
        glfwTerminate();
    }
    catch (const std::exception &e)
    {
        ERROR4(e.what());
    }

    return 0;
}