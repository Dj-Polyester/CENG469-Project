#define VERBOSE_ENABLED
#include "window.h"
#include "instance.h"
#include "physicalDevice.h"
#include "logicalDevice.h"

int main()
{

    try
    {
        // Use when using GLFW
        glfwInit();
        
        
        Window win(800, 600, "vulcano");
        Extension ext;
        Instance instance(win.name, ext);
        PhysicalDevice physicalDevice(instance.instance);
  
        LogicalDevice logicalDevice(instance.instance, physicalDevice.getPhysicalDeviceHandle());
        
        win.run();
        
        glfwTerminate();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}