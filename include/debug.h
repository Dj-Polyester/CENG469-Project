#pragma once

#include <sstream>
#include "types.h"

namespace debug
{
#ifdef VALIDATION_ENABLED
#define BLUE "\033[1;36m"
#define RED "\033[1;31m"
#define PRINT(stream, color, var) stream << "\033[1;37m" << __FILE__ << ":" << __LINE__ << ": " << color << var << "\033[0m " << std::endl
#define PRINT2(stream, color, label, var) stream << "\033[1;37m" << __FILE__ << ":" << __LINE__ << ": " << color << label << ":\033[0m " << var << std::endl

    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
#ifdef VERBOSE_ENABLED
    bool verbose_enabled = true;
#else
    bool verbose_enabled = false;
#endif
#ifdef INFO_ENABLED
    bool info_enabled = true;
#else
    bool info_enabled = false;
#endif

#if defined(VERBOSE_ENABLED) || defined(INFO_ENABLED)
#define DEBUG_INFO DEBUG
#define DEBUG_INFO2 DEBUG2
#define DEBUG_INFO3 DEBUG3
#define debugVkExtensions(ext) ext.queryExtensions()
#define debugVkLayers(ext) ext.queryLayers()
#define debugGlfwExtensions(ext) ext.queryGlfwExtensions()
#define debugPhysicalDevices(devManager) devManager.queryDevices()
#define debugPhysicalDevice(dev) dev.query()
#else
#define DEBUG_INFO
#define DEBUG_INFO2
#define DEBUG_INFO3
#define debugVkExtensions(ext)
#define debugVkLayers(ext)
#define debugGlfwExtensions(ext)
#define debugPhysicalDevices(devManager)
#define debugPhysicalDevice(dev)
#endif

#define DEBUG(var) PRINT(std::cerr, BLUE, var)
#define DEBUG2(label, var) PRINT2(std::cerr, BLUE, label, var)
#define DEBUG3(var) DEBUG2(#var, var)

#define ERROR(var)                          \
    {                                       \
        std::stringstream ss;               \
        PRINT(ss, RED, var);                \
        throw std::runtime_error(ss.str()); \
    }
#define ERROR2(label, var)                  \
    {                                       \
        std::stringstream ss;               \
        PRINT2(ss, RED, label, var);        \
        throw std::runtime_error(ss.str()); \
    }
#define ERROR3(var) ERROR2(#var, var)
#define ERROR4(str) ERROR2("error", str)

#define CERROR(var) PRINT(std::cerr, RED, var)
#define CERROR2(label, var) PRINT2(std::cerr, RED, label, var)
#define CERROR3(var) CERROR2(#var, var)
#define CERROR4(str) CERROR2("error", str)
#define debugVkResult(result)                            \
    {                                                    \
        std::string resultStr = getVkResult(result);     \
        if (resultStr == "VK_SUCCESS")                   \
        {                                                \
            DEBUG_INFO2("resultStr", resultStr.c_str()); \
        }                                                \
        else                                             \
        {                                                \
            ERROR2("resultStr", resultStr.c_str())       \
        }                                                \
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData)
    {

        DEBUG_INFO2("messageSeverity", getDebugUtilsMessageSeverity(messageSeverity));
        DEBUG_INFO2("messageType", getDebugUtilsMessageType(messageType));
        // debugVkpObjects(pCallbackData->pObjects, pCallbackData->objectCount);
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            CERROR3(pCallbackData->pMessage);
            return VK_FALSE;
        }
        DEBUG_INFO3(pCallbackData->pMessage);
        return VK_FALSE;
    }

#define debugLayerSupport(layers)                             \
    {                                                         \
        for (const char *layerName : debug::validationLayers) \
        {                                                     \
            bool layerFound = false;                          \
            for (const auto &layer : layers)                  \
            {                                                 \
                if (strcmp(layerName, layer.layerName) == 0)  \
                {                                             \
                    layerFound = true;                        \
                    break;                                    \
                }                                             \
            }                                                 \
            if (!layerFound)                                  \
            {                                                 \
                ERROR2(layerName, "Layer not found");         \
            }                                                 \
        }                                                     \
    }

#define enableValLayer(createInfo)                                                            \
    {                                                                                         \
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};                                 \
        createInfo.enabledLayerCount = static_cast<uint32_t>(debug::validationLayers.size()); \
        createInfo.ppEnabledLayerNames = debug::validationLayers.data();                      \
        debug::populateDebugMessengerCreateInfo(debugCreateInfo);                             \
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;            \
    }

    VkResult createDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT *pDebugCreateInfo,
        const VkAllocationCallbacks *pAllocator,
        VkDebugUtilsMessengerEXT *pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, pDebugCreateInfo, pAllocator, pDebugMessenger);
        }
        else
        {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    void destroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks *pAllocator)
    {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(instance, debugMessenger, pAllocator);
        }
    }

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &debugCreateInfo)
    {
        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        if (debug::verbose_enabled)
            debugCreateInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        if (debug::info_enabled)
            debugCreateInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = debug::debugCallback;
        debugCreateInfo.pUserData = nullptr;
    }

#define setupDebugMessenger(instance, debugMessenger)             \
    {                                                             \
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};     \
        debug::populateDebugMessengerCreateInfo(debugCreateInfo); \
        VkResult result = debug::createDebugUtilsMessengerEXT(    \
            instance,                                             \
            &debugCreateInfo,                                     \
            nullptr,                                              \
            &debugMessenger);                                     \
        debugVkResult(result);                                    \
    }
#define destroyDebugMessenger(instance, debugMessenger) debug::destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr)
#define glfwRequireDebugUtils(ext) ext.glfwRequireExt(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
#else

#define ERROR(var)
#define ERROR2(label, var)
#define ERROR3(var)
#define ERROR4(var)
#define ERROR4(str) ERROR2("error", str)
#define CERROR(var)
#define CERROR2(label, var)
#define CERROR3(var)
#define CERROR4(str)
#define debugVkResult(result)
#define debugLayerSupport(layers)
#define enableValLayer(createInfo)        \
    {                                     \
        createInfo.enabledLayerCount = 0; \
        createInfo.pNext = nullptr;       \
    }
#define setupDebugMessenger(instance, debugMessenger)
#define destroyDebugMessenger(instance, debugMessenger)
#define glfwRequireDebugUtils()

// VERBOSE
#define DEBUG(var)
#define DEBUG2(label, var)
#define DEBUG3(var)
#define debugVkExtensions(ext)
#define debugVkLayers(ext)
#define debugGlfwExtensions(ext)
#define debugPhysicalDevice(dev)
#define debugPhysicalDevices(devManager)
#endif
}
