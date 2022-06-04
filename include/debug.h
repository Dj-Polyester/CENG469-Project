#pragma once

#include <sstream>
#include "types.h"

namespace debug
{
#ifdef LITTLEBUG
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
#define DEBUG(var) std::cerr << "\033[1;37m" << __FILE__ << ":" << __LINE__ << "\033[1;36m " << var << "\033[0m " << std::endl
#define DEBUG2(label, var) std::cerr << "\033[1;37m" << __FILE__ << ":" << __LINE__ << ": \033[1;36m" << label << ":\033[0m " << var << std::endl
#define DEBUG3(var) DEBUG2(#var, var)
#else
#define DEBUG(var)
#define DEBUG2(label, var)
#define DEBUG3(var)
#endif

#define ERROR(var)                                                                                             \
    {                                                                                                          \
        std::stringstream ss;                                                                                  \
        ss << "\033[1;37m" << __FILE__ << ":" << __LINE__ << ": \033[1;31merror:\033[0m " << var << std::endl; \
        throw std::runtime_error(ss.str());                                                                    \
    }
#define ERROR2(label, var)                                                                                                      \
    {                                                                                                                           \
        std::stringstream ss;                                                                                                   \
        ss << "\033[1;37m" << __FILE__ << ":" << __LINE__ << ": \033[1;31merror:" << label << ": \033[0m " << var << std::endl; \
        throw std::runtime_error(ss.str());                                                                                     \
    }
#define ERROR3(var) ERROR2(#var, var)
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
            ERROR("extglfwExtCountCount is UINT32_MAX");                 \
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

#define debugpCallbackData(pCallbackData) \
    DEBUG3(pCallbackData->pMessage);      \
    debugVkpObjects(pCallbackData->pObjects, pCallbackData->objectCount)

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData)
    {

        DEBUG2("messageSeverity", getMessageSeverity(messageSeverity));
        debugpCallbackData(pCallbackData);

        if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            //std::cout << getMessageType(messageType) << std::endl;

            ERROR2("messageType", getMessageType(messageType));

            //ERROR2("messageType", getMessageType(messageType));

        }
        if (verbose_enabled || info_enabled)
            DEBUG2("messageType", getMessageType(messageType));
        return VK_FALSE;
    }

#define layerSupport(layerProps)                                       \
    {                                                                  \
        for (const char *layerName : debug::validationLayers)          \
        {                                                              \
            bool layerFound = false;                                   \
            for (const auto &layerProperties : layerProps)             \
            {                                                          \
                if (strcmp(layerName, layerProperties.layerName) == 0) \
                {                                                      \
                    layerFound = true;                                 \
                    break;                                             \
                }                                                      \
            }                                                          \
            if (!layerFound)                                           \
            {                                                          \
                ERROR2(layerName, "Layer not found");                  \
            }                                                          \
        }                                                              \
    }

#define glfwRequireVal(glfwExtensions, glfwExtCount)                 \
    {                                                                \
        glfwExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME); \
        ++glfwExtCount;                                              \
    }

#define createInfoLayerSet(createInfo)                                                        \
    {                                                                                         \
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};                                 \
        createInfo.enabledLayerCount = static_cast<uint32_t>(debug::validationLayers.size()); \
        createInfo.ppEnabledLayerNames = debug::validationLayers.data();                      \
        debug::populateDebugMessengerCreateInfo(debugCreateInfo);                             \
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;            \
    }

    VkResult createDebugUtilsMessengerEXT(
        VkInstance instance,
        const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
        const VkAllocationCallbacks *pAllocator,
        VkDebugUtilsMessengerEXT *pDebugMessenger)
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
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

    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
    {
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        if (debug::verbose_enabled)
            createInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
        if (debug::info_enabled)
            createInfo.messageSeverity |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;

        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debug::debugCallback;
        createInfo.pUserData = nullptr;
    }

#define setupDebugMessenger(instance, debugMessenger)          \
    {                                                          \
        VkDebugUtilsMessengerCreateInfoEXT createInfo{};       \
        debug::populateDebugMessengerCreateInfo(createInfo);   \
        VkResult result = debug::createDebugUtilsMessengerEXT( \
            instance,                                          \
            &createInfo,                                       \
            nullptr,                                           \
            &debugMessenger);                                  \
        debugVkResult(result);                                 \
    }
#define destroyDebugMessenger(instance, debugMessenger) debug::destroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr)

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
#define layerSupport(layerProps)
#define glfwRequireVal(glfwExtensions, glfwExtCount)
#define createInfoLayerSet(createInfo)    \
    {                                     \
        createInfo.enabledLayerCount = 0; \
        createInfo.pNext = nullptr;       \
    }
#define setupDebugMessenger(instance, debugMessenger)
#define destroyDebugMessenger(instance, debugMessenger)

#endif
}
