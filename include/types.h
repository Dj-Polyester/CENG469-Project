#pragma once

#include "includes.h"

std::string getMessageType(VkDebugUtilsMessageTypeFlagsEXT messageType);
std::string getMessageSeverity(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity);
std::string getVkResult(VkResult result);
std::string getStructureType(VkStructureType stype);