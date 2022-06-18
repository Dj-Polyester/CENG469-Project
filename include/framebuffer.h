#pragma once

struct Framebuffer
{
    std::vector<VkFramebuffer> swapChainFramebuffers;

    Framebuffer()
    {
    }
};
