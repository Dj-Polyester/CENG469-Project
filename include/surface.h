#pragma once

#include "extension.h"
#include <cstring>
#include <vulkan/vulkan.h>

struct Surface
{
	VkSurfaceKHR surface;
	VkInstance instance;
	GLFWwindow* window;

	Surface(VkInstance _instance, GLFWwindow* _window)
	{
		
		instance = _instance;
		window = _window;

		VkWin32SurfaceCreateInfoKHR createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = glfwGetWin32Window(window);
		createInfo.hinstance = GetModuleHandle(nullptr);

		if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}

	}

	~Surface()
	{
		vkDestroySurfaceKHR(instance, surface, nullptr);
	}

};