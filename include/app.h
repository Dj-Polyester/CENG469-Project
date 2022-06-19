#pragma once

#include "window.h"
#include "pipeline.h"

struct App
{

    Window win;
    Pipeline pipeline;

    App()
        : win{800, 600, "hello"},
          pipeline{"./shaders/spv/tri_vert.spv", "./shaders/spv/tri_frag.spv"}
    {
    }
    ~App()
    {
    }
    void run()
    {
        while (!glfwWindowShouldClose(win.window))
        {
            glfwPollEvents();
        }
    }
};
