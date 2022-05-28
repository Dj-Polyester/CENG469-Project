#include "window.h"

int main()
{
    try
    {
        // Use when using GLFW
        glfwInit();

        Window win(800, 600, "vulcano");
        win.run();
        glfwTerminate();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}