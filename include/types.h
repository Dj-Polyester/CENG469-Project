#pragma once

#include "includes.h"

std::string getDebugUtilsMessageType(VkDebugUtilsMessageTypeFlagsEXT messageType);
std::string getDebugUtilsMessageSeverity(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity);
std::string getVkResult(VkResult result);
std::string getVkStructureType(VkStructureType stype);
std::string getVkPhysicalDeviceType(VkPhysicalDeviceType pdtype);