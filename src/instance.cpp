#include "instance.h"

Instance::Instance(const std::string &name_) : name(name_)
{
#ifdef VALIDATION_ENABLED
    if (!checkValidationLayerSupport())
    {
        ERROR("validation layers requested, but not available!");
    }
#endif
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = name.c_str();
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensions.size());
    createInfo.ppEnabledExtensionNames = requiredExtensions.data();

#ifdef VALIDATION_ENABLED
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;

    createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
    createInfo.ppEnabledLayerNames = validationLayers.data();

    DebugMessenger::populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;

#else

    createInfo.enabledLayerCount = 0;
    createInfo.pNext = nullptr;

#endif
    if (vkCreateInstance(&createInfo, nullptr, &vkobject) != VK_SUCCESS)
    {
        ERROR("failed to create instance!");
    }

    hasRequiredInstanceExtensions();

#ifdef VALIDATION_ENABLED
    debugMessenger = new DebugMessenger(vkobject);
#endif
}
Instance::~Instance()
{
#ifdef VALIDATION_ENABLED
    delete debugMessenger;
#endif
    vkDestroyInstance(vkobject, nullptr);
}

bool Instance::checkValidationLayerSupport()
{
    vkEnumerateInstanceLayerProperties(&availableLayerCount, nullptr);

    availableLayers.resize(availableLayerCount);
    vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayers.data());

    for (const char *layerName : validationLayers)
    {
        bool layerFound = false;

        for (const auto &layerProperties : availableLayers)
        {
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            return false;
        }
    }

    return true;
}

void Instance::getRequiredExtensions()
{
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&requiredExtensionCount);

    requiredExtensions.insert(requiredExtensions.begin(), glfwExtensions, glfwExtensions + requiredExtensionCount);
#ifdef VALIDATION_ENABLED
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
}

void Instance::hasRequiredInstanceExtensions()
{
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, nullptr);
    availableExtensions.resize(availableExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &availableExtensionCount, availableExtensions.data());

    std::cout << "available extensions:" << std::endl;
    std::unordered_set<std::string> available;
    for (const auto &extension : availableExtensions)
    {
        std::cout << "\t" << extension.extensionName << std::endl;
        available.insert(extension.extensionName);
    }

    std::cout << "required extensions:" << std::endl;
    for (const auto &required : requiredExtensions)
    {
        std::cout << "\t" << required << std::endl;
        if (available.find(required) == available.end())
        {
            ERROR("Missing required glfw extension");
        }
    }
}