#include "B33Rendering.hpp"

#include "Vulkan/Instance.hpp"

namespace B33::Rendering
{

using namespace std;

// Instance // ---------------------------------------------------------------------------------------------------------
Instance::Instance()
    : m_Instance(CreateInstance())
{
    AB_LOG(Core::Debug::Info, L"Creating an instance!");
}

// ---------------------------------------------------------------------------------------------------------------------
Instance::~Instance()
{ 
    AB_LOG(Core::Debug::Info, L"Destroying instance");
    if (m_Instance != VK_NULL_HANDLE) {
        vkDestroyInstance(m_Instance, NULL);
        m_Instance = VK_NULL_HANDLE;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                    void* pUserData)
{
    constexpr wchar_t pwszFormat[] =
#ifdef _WIN32
        L"[Vulkan]: %S";
#elif __linux__
        L"[Vulkan]: %s";
#endif // !_WIN32

    ::Core::Debug::Logger::Get().Log(Core::Debug::Info, pwszFormat, pCallbackData->pMessage);
    return VK_FALSE;
}

// Private // ----------------------------------------------------------------------------------------------------------
VkInstance Instance::CreateInstance()
{ 
    VkInstance instance;
    VkResult   result;

#ifdef _DEBUG
    vector<VkValidationFeatureEnableEXT> enabledVaditationFeatures = {
        VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,
        VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT,
        VK_VALIDATION_FEATURE_ENABLE_BEST_PRACTICES_EXT,
        VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT,
        VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT,
    };
    vector<VkValidationFeatureDisableEXT> disabledVaditationFeatures = {
        VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT,
    };

    VkValidationFeaturesEXT validationFeatures = { };
    validationFeatures.sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT;
    validationFeatures.enabledValidationFeatureCount    = static_cast<uint32_t>(enabledVaditationFeatures.size());
    validationFeatures.pEnabledValidationFeatures       = &enabledVaditationFeatures[0];
    validationFeatures.disabledValidationFeatureCount   = static_cast<uint32_t>(disabledVaditationFeatures.size());
    validationFeatures.pDisabledValidationFeatures      = &disabledVaditationFeatures[0];

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = { };
    debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debugCreateInfo.pNext = &validationFeatures;
    debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
        VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debugCreateInfo.pfnUserCallback = debugCallback;
#endif // !_DEBUG


    const vector<const char*> vpszValidationLayers = {
#ifdef _DEBUG
        "VK_LAYER_KHRONOS_validation",
#endif // !_DEBUG
    };

    const vector<const char*> vpszExtensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
#ifdef _WIN32
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif __linux__
        VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#endif // !_WIN32

#ifdef _DEBUG
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
#endif
    };

    VkApplicationInfo appInfo = { };
    appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName    = "B33::Rendering";
    appInfo.applicationVersion  = VK_MAKE_VERSION(0, 1, 5);
    appInfo.pEngineName         = "AtlanticBeast";
    appInfo.engineVersion       = VK_MAKE_VERSION(0, 2, 0);
    appInfo.apiVersion          = VK_API_VERSION_1_1;

    VkInstanceCreateInfo createInfo = { };
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = 
#ifdef _DEBUG
        &debugCreateInfo;
#else
        NULL;
#endif
    createInfo.pApplicationInfo         = &appInfo;
    createInfo.ppEnabledLayerNames      = !vpszValidationLayers.empty() ? &vpszValidationLayers[0] : nullptr;
    createInfo.enabledLayerCount        = static_cast<uint32_t>(vpszValidationLayers.size());
    createInfo.ppEnabledExtensionNames  = !vpszExtensions.empty() ? &vpszExtensions[0] : nullptr;
    createInfo.enabledExtensionCount    = static_cast<uint32_t>(vpszExtensions.size());

    result = vkCreateInstance(&createInfo,
                              NULL,
                              &instance);

    if (result != VK_SUCCESS) {
        AB_LOG(Core::Debug::Error, L"Ohh nooo... Vulkan isn't working!!! Error code is: %d", result);
        throw AB_EXCEPT("Ohh nooo... Vulkan isn't working!!!");
    }

    return instance;
}

} // !B33::Rendering

