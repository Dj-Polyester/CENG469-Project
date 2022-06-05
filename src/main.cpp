#include "window.h"
#include "instance.h"
#include "physical_device.h"
int main()
{
    try
    {
        // Use when using GLFW
        glfwInit();

        Window win(800, 600, "vulcano");
        Extension ext;
        Instance instance(win.name, ext);
        PhysicalDeviceManager pdevman(instance);
        pdevman.pickBestScore();
        win.run();
        glfwTerminate();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}