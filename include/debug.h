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

#define DEBUG(var) PRINT(std::cerr, BLUE, var)
#define DEBUG2(label, var) PRINT2(std::cerr, BLUE, label, var)
#define DEBUG3(var) DEBUG2(#var, var)
#else
#define DEBUG(var)
#define DEBUG2(label, var)
#define DEBUG3(var)
#endif

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
#define CERROR(var) PRINT(std::cerr, RED, var)
#define CERROR2(label, var) PRINT2(std::cerr, RED, label, var)
#define CERROR3(var) CERROR2(#var, var)
#define debugVkResult(result)                        \
    {                                                \
        std::string resultStr = getVkResult(result); \
        if (resultStr == "VK_SUCCESS")               \
        {                                            \
            DEBUG2("resultStr", VK_SUCCESS);         \
        }                                            \
        else                                         \
        {                                            \
            ERROR2("resultStr", resultStr.c_str())   \
        }                                            \
    }

#define debugVkExtensions(extProps, extCount)                   \
    {                                                           \
        DEBUG2("Available extensions (extCount):", extCount);   \
        if (extCount == UINT32_MAX)                             \
        {                                                       \
            ERROR("extCount is UINT32_MAX");                    \
        }                                                       \
        for (size_t i = 0; i < extCount; ++i)                   \
        {                                                       \
            DEBUG(i);                                           \
            DEBUG2("extensionName", extProps[i].extensionName); \
            DEBUG2("specVersion", extProps[i].specVersion);     \
        }                                                       \
    }

#define debugVkLayers(layerProps, layerCount)                                     \
    {                                                                             \
        DEBUG2("Available layers (layerCount)", layerCount);                      \
        if (layerCount == UINT32_MAX)                                             \
        {                                                                         \
            ERROR("layerCount is UINT32_MAX");                                    \
        }                                                                         \
        for (size_t i = 0; i < layerCount; ++i)                                   \
        {                                                                         \
            DEBUG(i);                                                             \
            DEBUG2("layerName", layerProps[i].layerName);                         \
            DEBUG2("specVersion", layerProps[i].specVersion);                     \
            DEBUG2("implementationVersion", layerProps[i].implementationVersion); \
            DEBUG2("description", layerProps[i].description);                     \
        }                                                                         \
    }

#define debugGlfwExtensions(glfwExtensions, glfwExtCount)                \
    {                                                                    \
        DEBUG2("Required extensions glfw (glfwExtCount)", glfwExtCount); \
        if (glfwExtCount == UINT32_MAX)                                  \
        {                                                                \
            ERROR("glfwExtCount is UINT32_MAX");                         \
        }                                                                \
        for (size_t i = 0; i < glfwExtCount; ++i)                        \
        {                                                                \
            DEBUG2(i, glfwExtensions[i]);                                \
        }                                                                \
    }

#define debugVkpObjects(pObjects, objectCount)                     \
    {                                                              \
        DEBUG2("pObjects objectCount (objectCount)", objectCount); \
        for (size_t i = 0; i < objectCount; ++i)                   \
        {                                                          \
            DEBUG(i);                                               \
            if(pObjects[i].pObjectName)                             \
                DEBUG2("pObjectName", pObjects[i].pObjectName);        \
            DEBUG2("sType", getStructureType(pObjects[i].sType));  \
            DEBUG2("objectType", pObjects[i].objectType);          \
            DEBUG2("objectHandle", pObjects[i].objectHandle);      \
        }                                                          \
    }

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData)
    {

        DEBUG2("messageSeverity", getMessageSeverity(messageSeverity));
        DEBUG2("messageType", getMessageType(messageType));
        // debugVkpObjects(pCallbackData->pObjects, pCallbackData->objectCount);
        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            CERROR3(pCallbackData->pMessage);
            return VK_FALSE;
        }
        DEBUG3(pCallbackData->pMessage);
        return VK_FALSE;
    }

#define debugLayerSupport(layerProps)                            \
    {                                                            \
        for (const char *layerName : debug::validationLayers)    \
        {                                                        \
            bool layerFound = false;                             \
            for (const auto &layerProp : layerProps)             \
            {                                                    \
                if (strcmp(layerName, layerProp.layerName) == 0) \
                {                                                \
                    layerFound = true;                           \
                    break;                                       \
                }                                                \
            }                                                    \
            if (!layerFound)                                     \
            {                                                    \
                ERROR2(layerName, "Layer not found");            \
            }                                                    \
        }                                                        \
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
#define glfwRequireDebugUtils() glfwRequire(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
#else

#define DEBUG(var)
#define DEBUG2(label, var)
#define DEBUG3(var)
#define DEBUG4(var)
#define ERROR(var)
#define ERROR2(label, var)
#define ERROR3(var)
#define ERROR4(var)
#define debugVkResult(result)
#define debugVkLayers(layerProps, layerCount)
#define debugVkExtensions(extProps, extCount)
#define debugGlfwExtensions(glfwExtensions, glfwExtCount)
#define debugLayerSupport(layerProps)
#define enableValLayer(createInfo)        \
    {                                     \
        createInfo.enabledLayerCount = 0; \
        createInfo.pNext = nullptr;       \
    }
#define setupDebugMessenger(instance, debugMessenger)
#define destroyDebugMessenger(instance, debugMessenger)
#define glfwRequireDebugUtils()
#endif
}
