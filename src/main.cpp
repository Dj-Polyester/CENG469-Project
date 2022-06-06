#include "instance.h"
#include "physical_device.h"
#include "logical_device.h"
int main()
{
    try
    {
        // Use when using GLFW
        glfwInit();
        App app("testProj");
        Extension ext;
        Instance instance(ext, app, "vulcano");
        Window win(800, 600, instance);
        PhysicalDeviceManager pdevman(win, instance);
        LogicalDevice ldev(pdevman.bestDeviceOnScore(win.surface));

        win.run();
        glfwTerminate();
    }
    catch (const std::exception &e)
    {
        ERROR4(e.what());
    }

    return 0;
}