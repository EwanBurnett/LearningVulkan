#include "Helpers.h"
#include <vulkan/vk_enum_string_helper.h>
#include <stdexcept>
#include <set>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>

//LOAD EXTENSION FUNCTIONS
VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

VkResult DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT& debugMessenger, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
        return VK_SUCCESS;
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

VkResult Helpers::VkCall_Impl(const VkResult& result, const char* const file, const uint32_t line, const char* const function)
{
    switch (result) {
        //SUCCESS CODES
    case VK_SUCCESS:
    case VK_NOT_READY:
    case VK_TIMEOUT:
    case VK_EVENT_SET:
    case VK_EVENT_RESET:
    case VK_INCOMPLETE:
    case VK_SUBOPTIMAL_KHR:
    case VK_THREAD_IDLE_KHR:
    case VK_THREAD_DONE_KHR:
    case VK_OPERATION_DEFERRED_KHR:
    case VK_OPERATION_NOT_DEFERRED_KHR:
    case VK_PIPELINE_COMPILE_REQUIRED:
        break;
        //ERROR CODES
    case VK_ERROR_OUT_OF_HOST_MEMORY:
    case VK_ERROR_OUT_OF_DEVICE_MEMORY:
    case VK_ERROR_INITIALIZATION_FAILED:
    case VK_ERROR_DEVICE_LOST:
    case VK_ERROR_MEMORY_MAP_FAILED:
        //break;
    case VK_ERROR_LAYER_NOT_PRESENT:
        //Log::Error(file, line, function, "[VK_ERROR_LAYER_NOT_PRESENT] - One or more of the requested Layers were not present!\n");
        //break;
    case VK_ERROR_EXTENSION_NOT_PRESENT:
        //Log::Error(file, line, function, "[VK_ERROR_EXTENSION_NOT_PRESENT] - One or more of the requested Extensions were not present!\n");
        //break;
    case VK_ERROR_FEATURE_NOT_PRESENT:
    case VK_ERROR_INCOMPATIBLE_DRIVER:
    case VK_ERROR_TOO_MANY_OBJECTS:
    case VK_ERROR_FORMAT_NOT_SUPPORTED:
    case VK_ERROR_FRAGMENTED_POOL:
    case VK_ERROR_SURFACE_LOST_KHR:
    case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
    case VK_ERROR_OUT_OF_DATE_KHR:
    case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
    case VK_ERROR_INVALID_SHADER_NV:
    case VK_ERROR_OUT_OF_POOL_MEMORY:
    case VK_ERROR_INVALID_EXTERNAL_HANDLE:
    case VK_ERROR_FRAGMENTATION:
    case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT:
        //case VK_ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS:
    case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT:
    case VK_ERROR_VALIDATION_FAILED_EXT:
    case VK_ERROR_COMPRESSION_EXHAUSTED_EXT:
        //case VK_ERROR_IMAGE_USAGE_NOT_SUPPORTED_KHR:
        //case VK_ERROR_VIDEO_PICTURE_LAYOUT_NOT_SUPPORTED_KHR:
        //case VK_ERROR_VIDEO_PROFILE_OPERATION_NOT_SUPPORTED_KHR:
        //case VK_ERROR_VIDEO_PROFILE_FORMAT_NOT_SUPPORTED_KHR:
        //case VK_ERROR_VIDEO_PROFILE_CODEC_NOT_SUPPORTED_KHR:
       // case VK_ERROR_VIDEO_STD_VERSION_NOT_SUPPORTED_KHR:
            //case VK_ERROR_INVALID_VIDEO_STD_PARAMETERS_KHR:
        //case VK_ERROR_INCOMPATIBLE_SHADER_BINARY_EXT:
        Log::Error(file, line, function, string_VkResult(result));
        Log::Print("\n");
        break;
    case VK_ERROR_UNKNOWN:
    default:
        Log::Fatal(file, line, function, false, "A Fatal Error has occurred in a Vulkan Call.");
        break;
    }
    return result;
}

//Creates a VkInstance handle based on the supplied information. 
VkInstance Helpers::CreateInstance(const char* const appName, const uint32_t appVersion, const char** const pInstanceLayers, const uint32_t numInstanceLayers, const char** const pInstanceExtensions, const uint32_t numInstanceExtensions, const VkAllocationCallbacks* pAllocator)
{
    Log::Message("Creating Vulkan Instance.\n");

    //Throw an exception if the input data is invalid. 
    if (numInstanceLayers > 0 && pInstanceLayers == nullptr) {
        throw std::invalid_argument("Invalid Array!\nnumInstanceLayers was > 0, but pInstanceLayers was nullptr!\n");
    }

    if (numInstanceExtensions > 0 && pInstanceExtensions == nullptr) {
        throw std::invalid_argument("Invalid Array!\nnumInstanceExtensions was > 0, but pInstanceExtensions was nullptr!\n");
    }
    
    //Validate each Layer and Extension

    if(pInstanceLayers){
    bool layersValid = Helpers::ValidateArrayInstanceLayerSupport(pInstanceLayers, numInstanceLayers);
    if (!layersValid) {
        throw std::runtime_error("Not all Instance Layers were supported!");
    }
    }
    bool extensionsValid = Helpers::ValidateArrayInstanceExtensionSupport(pInstanceExtensions, numInstanceExtensions);
    if (!extensionsValid) {
        throw std::runtime_error("Not all Instance Extensions were supported!");
    }
    
    //Populate a VkApplicationInfo struct with the app info. 
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext = nullptr;
    appInfo.pApplicationName = appName;
    appInfo.applicationVersion = appVersion;
    appInfo.pEngineName = "NONE";
    appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_3;

    //Populate a VkInstanceCreateInfo struct with our application, layer and extension data. 
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.flags = 0;
    createInfo.enabledExtensionCount = numInstanceExtensions;
    createInfo.ppEnabledExtensionNames = pInstanceExtensions;
    createInfo.enabledLayerCount = numInstanceLayers;
    createInfo.ppEnabledLayerNames = pInstanceLayers;

    //Create the Instance Handle. 
    VkInstance instance = {};
    auto res = VK_CALL(vkCreateInstance(&createInfo, pAllocator, &instance));

    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Vulkan Instance!\n");
    }

    Log::Success("VkInstance created at [0x%08x]!\n", instance);
    return instance;
}

VkDevice Helpers::CreateLogicalDevice(const VkPhysicalDevice& physicalDevice, const QueueFamilyIndices& queueFamilyIndices, const SwapChainSupportDetails& swapChainSupport, const char** const pDeviceExtensions ,const uint32_t numDeviceExtensions, const VkAllocationCallbacks* pAllocator )
{
    Log::Message("Creating Logical Device.\n");

   
    bool extensionsValid = Helpers::ValidateArrayDeviceExtensionSupport(physicalDevice, pDeviceExtensions, numDeviceExtensions);
    if (!extensionsValid) {
        throw std::runtime_error("Not all Device Extensions were supported!");
    }

    //We want Graphics and Presentation support - so make queues from those families. 
    std::vector<VkDeviceQueueCreateInfo> pQueueCreateInfo; 
    {
        std::vector<int> evaluatedFamilies;

        auto evalQueueFamily = [&](uint32_t idx, uint32_t count) {
            //If we've already evaluated this queue family, then break. 
            if (std::find(evaluatedFamilies.begin(), evaluatedFamilies.end(), idx) != evaluatedFamilies.end()) {
                return;
            }

            evaluatedFamilies.push_back(idx);

            float* pPriorities = new float[count];  //This will be destroyed later. 
            for (int i = 0; i < count; i++) {
                pPriorities[i] = 1.0f;
            }

            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.pNext = nullptr;
            queueCreateInfo.queueFamilyIndex = idx;
            queueCreateInfo.queueCount = count;
            queueCreateInfo.pQueuePriorities = pPriorities;

            pQueueCreateInfo.push_back(queueCreateInfo);
        };

        for (auto& pair : queueFamilyIndices.Graphics) {

            const auto& idx = pair.first;
            const auto& count = pair.second;

            evalQueueFamily(idx, count);

        }

        for (auto& pair : queueFamilyIndices.Presentation) {
            const auto& idx = pair.first;
            const auto& count = pair.second;

            evalQueueFamily(idx, count);
        }
    }

    //Now check if the SwapChain is supported...
    {
       
        if (swapChainSupport.formats.empty() || swapChainSupport.presentMode.empty())
        {
            throw std::runtime_error("Swap chain not supported!\n");
        }


    }


    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pNext = nullptr;
    createInfo.pQueueCreateInfos = pQueueCreateInfo.data();
    createInfo.queueCreateInfoCount = pQueueCreateInfo.size();
    createInfo.enabledExtensionCount = numDeviceExtensions;
    createInfo.ppEnabledExtensionNames = pDeviceExtensions;


    VkDevice device = {};
    auto res = VK_CALL(vkCreateDevice(physicalDevice, &createInfo, pAllocator, &device));
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Logical Device!\n");
    }

    //Clean up our arrays. 
    for (auto& family : pQueueCreateInfo) {
        delete[] family.pQueuePriorities;
    }

    Log::Success("Logical Device Created! [0x%08x]\n", device);
    return device;
}

VkPhysicalDevice Helpers::AcquirePhysicalDevice(const VkInstance& instance, const VkPhysicalDeviceProperties* pProperties, const VkPhysicalDeviceFeatures* pFeatures)
{
    Log::Message("Selecting a Physical Device.\n");

    if (instance == nullptr) {
        throw std::runtime_error("Vulkan Instance was Invalid!\n");
    }

    uint32_t deviceIdx = 0;
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    VkPhysicalDevice* pDevices = new VkPhysicalDevice[deviceCount];
    vkEnumeratePhysicalDevices(instance, &deviceCount, pDevices);

    VkPhysicalDevice device = {};
    if (deviceCount < 1) {
        throw std::runtime_error("No Physical Devices Found!\n");
    }
    else if (deviceCount == 1) {    //If there's only one device, just use that. 
        device = pDevices[0];
        deviceIdx = 0;
    }
    else {
        //TODO: Score each physical device, and select the one with the highest score. 
        device = pDevices[0];
        deviceIdx = 0;
    }

    Log::Success("Physical Device %d Selected!\n", deviceIdx);
    return device;
}

uint32_t Helpers::EvaluatePhysicalDeviceSuitability(const VkPhysicalDevice& device, const VkPhysicalDeviceProperties* pProperties, const VkPhysicalDeviceFeatures* pFeatures)
{
    return 0;   //TODO: Physical device feature ranking
}

Helpers::QueueFamilyIndices Helpers::GetQueueFamilyProperties(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR* pSurface)
{
    QueueFamilyIndices idx;
    uint32_t numQueueFamilies = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &numQueueFamilies, nullptr);
    VkQueueFamilyProperties* pQueueFamilyProperties = new VkQueueFamilyProperties[numQueueFamilies];
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &numQueueFamilies, pQueueFamilyProperties);

    for (int i = 0; i < numQueueFamilies; i++) {
        const uint32_t flags = pQueueFamilyProperties[i].queueFlags;
        const uint32_t queueCount = pQueueFamilyProperties[i].queueCount;

        if (flags & VK_QUEUE_GRAPHICS_BIT) {
            idx.Graphics.insert(std::make_pair(i, queueCount));
        }

        if (flags & VK_QUEUE_COMPUTE_BIT) {
            idx.Compute.insert(std::make_pair(i, queueCount));
        }

        if (flags & VK_QUEUE_TRANSFER_BIT) {
            idx.Transfer.insert(std::make_pair(i, queueCount));
        }

        if (flags & VK_QUEUE_SPARSE_BINDING_BIT) {
            idx.Sparse.insert(std::make_pair(i, queueCount));
        }

        if (pSurface != nullptr) {
            VkBool32 supportsPresentation = false;
            VK_CALL(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, *pSurface, &supportsPresentation));
            if (supportsPresentation) {
                idx.Presentation.insert(std::make_pair(i, queueCount));
            }
        }
    }

    return idx;
}



VkSurfaceKHR Helpers::CreateWindowSurface(const VkInstance& instance, GLFWwindow* window, const VkAllocationCallbacks* pAllocator)
{
    Log::Message("Creating Window Surface\n");
    VkSurfaceKHR surface = {};
#ifdef USE_WIN32
    VkWin32SurfaceCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = glfwGetWin32Window(window);
    createInfo.hinstance = GetModuleHandle(nullptr);

    auto res = VK_CALL(vkCreateWin32SurfaceKHR(instance, &createInfo, pAllocator, &surface));
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Unable to create Window Surface (win32)!");
    }

#else


    auto res = VK_CALL(glfwCreateWindowSurface(instance, window, nullptr, &surface));
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Unable to create Window Surface (glfw)!");
    }
#endif
    Log::Success("Window Surface Created at [0x%08x]\n", surface);
    return surface;
}

void Helpers::DestroyWindowSurface(const VkInstance& instance, const VkSurfaceKHR& surface, const VkAllocationCallbacks* pAllocator)
{
    Log::Message("Destroying Window Surface");
    vkDestroySurfaceKHR(instance, surface, pAllocator);
}

bool Helpers::ValidateDeviceExtensionSupport(const VkPhysicalDevice& device, const char* const extension, VkExtensionProperties* pExtensionProperties, const uint32_t extensionPropertyCount)
{
    Log::Message("Validating Device extension %s.\n", extension);

    bool supported = false;

    VkExtensionProperties* instanceSupportedExtensions = nullptr;
    uint32_t numDeviceSupportedExtensions = 0;

    if (pExtensionProperties == nullptr || extensionPropertyCount <= 0)
    {
        //Allocate the instanceSupportedExtensions array. 
        vkEnumerateDeviceExtensionProperties(device, nullptr, &numDeviceSupportedExtensions, nullptr);
        instanceSupportedExtensions = new VkExtensionProperties[numDeviceSupportedExtensions];
        vkEnumerateDeviceExtensionProperties(device, nullptr, &numDeviceSupportedExtensions, instanceSupportedExtensions);
    }
    else {  //If extension properties have already been evaluated, Then we don't need to re-evaluate them.
        instanceSupportedExtensions = pExtensionProperties;
        numDeviceSupportedExtensions = extensionPropertyCount;
    }

    //Search the array for the extension. If found, then the extension is supported. 
    if (instanceSupportedExtensions == nullptr) {
        throw std::runtime_error("Device Supported Extensions array was nullptr!\n");
    }

    for (uint32_t i = 0; i < numDeviceSupportedExtensions; i++)
    {
        if (strcmp(instanceSupportedExtensions[i].extensionName, extension) == 0) {
            Log::Success("Extension %s is supported by the current Physical Device!\n", extension);
            supported = true;
            goto ExtensionFound;
        }

    }

    //Report if we didn't find the extension. 
    Log::Failure("Extension %s was not supported by the current Physical Device.\n", extension);


ExtensionFound:
    //If we allocated the instanceSupportedExtensions array internally, be sure to free it. 
    if (pExtensionProperties == nullptr || extensionPropertyCount <= 0)
    {
        delete[] instanceSupportedExtensions;
    }

    return supported;
}

bool Helpers::ValidateArrayDeviceExtensionSupport(const VkPhysicalDevice& device, const char** const pExtensions, const uint32_t numExtensions)
{
    bool result = true;

    if (pExtensions == nullptr || numExtensions == 0) {
        Log::Warning("ValidateArrayDeviceExtensionSupport() was called, but pExtensions was [0x%08x], and numExtensions was %d!\nDefaulting to False\n", pExtensions, numExtensions);
        return false;
    }

    //Check each extension against the device's supported extensions. 
    {
        //Evaluate the device supported extensions once here, and pass through to the Validation function. 
        uint32_t numDeviceExtensions = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &numDeviceExtensions, nullptr);
        VkExtensionProperties* deviceSupportedExtensions = new VkExtensionProperties[numDeviceExtensions];
        vkEnumerateDeviceExtensionProperties(device, nullptr, &numDeviceExtensions, deviceSupportedExtensions);

        for (uint32_t i = 0; i < numExtensions; i++) {
            result &= ValidateDeviceExtensionSupport(device, pExtensions[i], deviceSupportedExtensions, numDeviceExtensions);
        }

        //Ensure the array is properly deallocated. 
        delete[] deviceSupportedExtensions;
    }

    return result;
}

bool Helpers::ValidateInstanceExtensionSupport(const char* const extension, VkExtensionProperties* pExtensionProperties, const uint32_t extensionPropertyCount)
{
    Log::Message("Validating extension %s.\n", extension);

    bool supported = false;

    VkExtensionProperties* instanceSupportedExtensions = nullptr;
    uint32_t numInstanceSupportedExtensions = 0;

    if (pExtensionProperties == nullptr || extensionPropertyCount <= 0)
    {
        //Allocate the instanceSupportedExtensions array. 
        vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceSupportedExtensions, nullptr);
        instanceSupportedExtensions = new VkExtensionProperties[numInstanceSupportedExtensions];
        vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceSupportedExtensions, instanceSupportedExtensions);
    }
    else {  //If extension properties have already been evaluated, Then we don't need to re-evaluate them.
        instanceSupportedExtensions = pExtensionProperties;
        numInstanceSupportedExtensions = extensionPropertyCount;
    }

    //Search the array for the extension. If found, then the extension is supported. 
    if (instanceSupportedExtensions == nullptr) {
        throw std::runtime_error("Instance Supported Extensions array was nullptr!\n");
    }

    for (uint32_t i = 0; i < numInstanceSupportedExtensions; i++)
    {
        if (strcmp(instanceSupportedExtensions[i].extensionName, extension) == 0) {
            Log::Success("Extension %s is supported by the current Instance!\n", extension);
            supported = true;
            goto ExtensionFound;
        }

    }

    //Report if we didn't find the extension. 
    Log::Failure("Extension %s was not supported by the current Instance.\n", extension);


ExtensionFound:
    //If we allocated the instanceSupportedExtensions array internally, be sure to free it. 
    if (pExtensionProperties == nullptr || extensionPropertyCount <= 0)
    {
        delete[] instanceSupportedExtensions;
    }

    return supported;
}

bool Helpers::ValidateArrayInstanceExtensionSupport(const char** const pExtensions, const uint32_t numExtensions)
{
    bool result = true;

    if (pExtensions == nullptr || numExtensions == 0) {
        Log::Warning("ValidateArrayInstanceExtensionSupport() was called, but pExtensions was [0x%08x], and numExtensions was %d!\nDefaulting to False\n", pExtensions, numExtensions);
        return false;
    }

    //Check each extension against the instance's supported extensions. 
    {
        //Evaluate the instance supported extensions once here, and pass through to the Validation function. 
        uint32_t numInstanceExtensions = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceExtensions, nullptr);
        VkExtensionProperties* instanceSupportedExtensions = new VkExtensionProperties[numInstanceExtensions];
        vkEnumerateInstanceExtensionProperties(nullptr, &numInstanceExtensions, instanceSupportedExtensions);

        for (uint32_t i = 0; i < numExtensions; i++) {
            result &= ValidateInstanceExtensionSupport(pExtensions[i], instanceSupportedExtensions, numInstanceExtensions);
        }

        //Ensure the array is properly deallocated. 
        delete[] instanceSupportedExtensions;
    }

    return result;
}

bool Helpers::ValidateInstanceLayerSupport(const char* const layer, VkLayerProperties* pLayerProperties, const uint32_t layerPropertyCount)
{
    Log::Message("Validating layer %s.\n", layer);

    bool supported = false;

    VkLayerProperties* instanceSupportedLayers = nullptr;
    uint32_t numInstanceSupportedLayers = 0;

    if (pLayerProperties == nullptr || layerPropertyCount <= 0)
    {
        //Allocate the instanceSupportedLayers array. 
        vkEnumerateInstanceLayerProperties(&numInstanceSupportedLayers, nullptr);
        instanceSupportedLayers = new VkLayerProperties[numInstanceSupportedLayers];
        vkEnumerateInstanceLayerProperties(&numInstanceSupportedLayers, instanceSupportedLayers);
    }
    else {  //If layer properties have already been evaluated, Then we don't need to re-evaluate them.
        instanceSupportedLayers = pLayerProperties;
        numInstanceSupportedLayers = layerPropertyCount;
    }

    //Search the array for the layer. If found, then the layer is supported. 
    if (instanceSupportedLayers == nullptr) {
        throw std::runtime_error("Instance Supported Layers array was nullptr!\n");
    }

    for (uint32_t i = 0; i < numInstanceSupportedLayers; i++)
    {
        if (strcmp(instanceSupportedLayers[i].layerName, layer) == 0) {
            Log::Success("Layer %s is supported by the current Instance!\n", layer);
            supported = true;
            goto LayerFound;
        }

    }

    //Report if we didn't find the layer. 
    Log::Failure("Layer %s was not supported by the current Instance.\n", layer);


LayerFound:
    //If we allocated the instanceSupportedLayers array internally, be sure to free it. 
    if (pLayerProperties == nullptr || layerPropertyCount <= 0)
    {
        delete[] instanceSupportedLayers;
    }

    return supported;
}

bool Helpers::ValidateArrayInstanceLayerSupport(const char** const pLayers, const uint32_t numLayers)
{
    bool result = true;

    if (pLayers == nullptr || numLayers == 0) {
        Log::Warning("ValidateArrayInstanceLayerSupport() was called, but pLayers was [0x%08x], and numLayers was %d!\nDefaulting to False\n", pLayers, numLayers);
        return false;
    }

    //Check each layer against the instance's supported layers. 
    {
        //Evaluate the instance supported layers once here, and pass through to the Validation function. 
        uint32_t numInstanceLayers = 0;
        vkEnumerateInstanceLayerProperties(&numInstanceLayers, nullptr);
        VkLayerProperties* instanceSupportedLayers = new VkLayerProperties[numInstanceLayers];
        vkEnumerateInstanceLayerProperties(&numInstanceLayers, instanceSupportedLayers);

        for (uint32_t i = 0; i < numLayers; i++) {
            result &= ValidateInstanceLayerSupport(pLayers[i], instanceSupportedLayers, numInstanceLayers);
        }

        //Ensure the array is properly deallocated. 
        delete[] instanceSupportedLayers;
    }

    return result;
}

VkDebugUtilsMessengerEXT Helpers::CreateDebugLogger(const VkInstance& instance, const VkAllocationCallbacks* pAllocator)
{
    Log::Message("Creating a Debug Logger.\n");

    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = Helpers::DebugLog;
    createInfo.pUserData = nullptr;

    VkDebugUtilsMessengerEXT debugLogger;
    auto res = VK_CALL(CreateDebugUtilsMessengerEXT(instance, &createInfo, pAllocator, &debugLogger));

    Log::Success("Debug Logger created at [0x%08x] (allocator [0x%08x])!\n", &debugLogger, pAllocator);

    return debugLogger;
}

void Helpers::DestroyDebugLogger(const VkInstance& instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator)
{
    Log::Message("Destroying Debug Messenger [0x%08x].\n", &messenger);
    auto res = VK_CALL(DestroyDebugUtilsMessengerEXT(instance, messenger, pAllocator));

    if (res != VK_SUCCESS) {
        throw std::runtime_error("Unable to destroy Debug Messenger!\n");
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL Helpers::DebugLog(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    //Log warnings and errors to stdout. 
    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        Log::Warning(pCallbackData->pMessage);
        Log::Print("\n");
    }

    if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        Log::Error(__FILE__, __LINE__, __PRETTY_FUNCTION__, pCallbackData->pMessage);    //TODO: Retrieve from pUserData.
        Log::Print("\n");
    }

    return VK_FALSE;
}


void Helpers::DestroyInstance(VkInstance& instance, const VkAllocationCallbacks* pAllocator)
{
    Log::Message("Destroying Vulkan Instance at [0x%08x] (allocator [0x%08x])\n", instance, pAllocator);
    vkDestroyInstance(instance, pAllocator);
}

void Helpers::DestroyLogicalDevice(VkDevice& device, const VkAllocationCallbacks* pAllocator)
{
    Log::Message("Destroying Logical Device [0x%08x]", device);

    vkDestroyDevice(device, pAllocator);
}

Helpers::SwapChainSupportDetails Helpers::QuerySwapChainSupport(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
    SwapChainSupportDetails details;

    VkSurfaceCapabilitiesKHR surfaceCapabilities = {};

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
    details.capabilities = surfaceCapabilities;

    

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
    details.presentMode.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentMode.data());


    return details;
}

VkSurfaceFormatKHR Helpers::SelectSwapChainFormat(const uint32_t numFormats, const VkSurfaceFormatKHR* pFormats)
{
    for (int i = 0; i < numFormats; i++) {
        if (pFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB && pFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return pFormats[i];
        }
    }

    return pFormats[0];
}

VkPresentModeKHR Helpers::SelectSwapChainPresentMode(const uint32_t numPresentModes, const VkPresentModeKHR* pModes)
{

    for (int i = 0; i < numPresentModes; i++) {
        if (pModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            return pModes[i];
        }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D Helpers::SelectSwapChainExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window)
{

    //TODO: Evaluate this properly. 

    VkExtent2D extent = {};

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    extent.width = width;
    extent.height = height;

    return extent;
}

VkSwapchainKHR Helpers::CreateSwapChain(const VkDevice& device, const VkSurfaceKHR& surface, const VkSurfaceFormatKHR& format, const VkPresentModeKHR& presentMode, const VkExtent2D& extents, const VkSurfaceCapabilitiesKHR capabilities, const VkAllocationCallbacks* pAllocator)
{
    Log::Message("Creating Swapchain\n");

    uint32_t imageCount = capabilities.minImageCount + 1;

    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.pNext = nullptr;
    createInfo.surface = surface;
    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = format.format;
    createInfo.imageColorSpace = format.colorSpace;
    createInfo.imageExtent = extents;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;    //NOTE: Using this as a default, may cause issues. 
    createInfo.preTransform = capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    

    VkSwapchainKHR swapChain = {};
    auto res = VK_CALL(vkCreateSwapchainKHR(device, &createInfo, pAllocator, &swapChain));
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Unable to create Swapchain!\n");
    }

    return swapChain;
}

void Helpers::DestroySwapChain(const VkDevice& device, VkSwapchainKHR& swapChain)
{
    vkDestroySwapchainKHR(device, swapChain, nullptr); //TODO: pAllocator   
}

std::vector<VkImageView> Helpers::CreateImageViews(const VkDevice& device, const std::vector<VkImage>& swapChainImages, const VkFormat& swapChainImageFormat, const VkAllocationCallbacks* pAllocator)
{
    std::vector<VkImageView> views;

    views.resize(swapChainImages.size());

    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0; 
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        auto res = VK_CALL(vkCreateImageView(device, &createInfo, nullptr, &views[i]));
        if (res != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Image Views!");
        }
    }


    return views;
}

void Helpers::DestroyImageView(const VkDevice& device, VkImageView imageView, const VkAllocationCallbacks* pAllocator)
{
    vkDestroyImageView(device, imageView, pAllocator);
}

VkPipeline Helpers::CreateGraphicsPipeline(const VkDevice& device, const VkShaderModule& vs, const VkShaderModule& fs, const VkExtent2D& swapchainExtents, const VkPipelineLayout& layout, const VkRenderPass& renderPass, const VkAllocationCallbacks* pAllocator)
{

    VkPipelineDynamicStateCreateInfo dynamicState = {};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 0;
    dynamicState.pDynamicStates = nullptr; 

    VkPipelineShaderStageCreateInfo vs_CreateInfo = {};
    vs_CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vs_CreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vs_CreateInfo.module = vs;
    vs_CreateInfo.pName = "main";   //Entry Point
   


    VkPipelineShaderStageCreateInfo fs_CreateInfo = {};
    fs_CreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fs_CreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fs_CreateInfo.module = fs;
    fs_CreateInfo.pName = "main";   //Entry Point

    VkPipelineShaderStageCreateInfo stages[] = { vs_CreateInfo, fs_CreateInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.pNext = nullptr;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr;
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr;
    

    VkPipelineInputAssemblyStateCreateInfo sci = {};
    sci.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    sci.pNext = nullptr;
    sci.primitiveRestartEnable = VK_FALSE;
    sci.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkViewport viewport = {};
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = (float)swapchainExtents.width;
    viewport.height = (float)swapchainExtents.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    scissor.offset = { 0, 0 };
    scissor.extent = swapchainExtents;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo RS = {};
    RS.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    RS.depthClampEnable = VK_FALSE;
    RS.rasterizerDiscardEnable = VK_FALSE;
    RS.polygonMode = VK_POLYGON_MODE_FILL;
    RS.cullMode = VK_CULL_MODE_BACK_BIT;
    RS.frontFace = VK_FRONT_FACE_CLOCKWISE;
    RS.depthBiasEnable = VK_FALSE;
    RS.depthBiasConstantFactor = 0.0f;
    RS.depthBiasClamp = 0.0f;
    RS.depthBiasSlopeFactor = 0.0f;
    RS.lineWidth = 1.0f;

    VkPipelineMultisampleStateCreateInfo msaa = {};
    msaa.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    msaa.sampleShadingEnable = VK_FALSE;
    msaa.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    msaa.minSampleShading = 1.0f;
    msaa.pSampleMask = nullptr;
    msaa.alphaToCoverageEnable = VK_FALSE;
    msaa.alphaToOneEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional


   
    VkGraphicsPipelineCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO; 
    createInfo.stageCount = 2;
    createInfo.pStages = stages;
    createInfo.pVertexInputState = &vertexInputInfo; 
    createInfo.pInputAssemblyState = &sci;
    createInfo.pViewportState = &viewportState;
    createInfo.pRasterizationState = &RS;
    createInfo.pMultisampleState = &msaa;
    createInfo.pDepthStencilState = nullptr;
    createInfo.pColorBlendState = &colorBlending;
    createInfo.pDynamicState = &dynamicState; 
    createInfo.layout = layout;
    createInfo.renderPass = renderPass;
    createInfo.subpass = 0;
    createInfo.basePipelineHandle = VK_NULL_HANDLE;
    createInfo.basePipelineIndex = -1;

    VkPipeline pipeline = {};
    auto r = VK_CALL(vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &pipeline));
    if (r != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Graphics Pipeline!");
    }

    return pipeline;


}

void Helpers::DestroyPipeline(const VkDevice& device, VkPipeline pipeline)
{
    vkDestroyPipeline(device, pipeline, nullptr);
}

std::vector<char> Helpers::ReadShaderBlob(const char* fileName)
{
    std::ifstream file(fileName, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file!\n");
    }

    size_t fileSize = (size_t)file.tellg(); 
    std::vector<char> buffer(fileSize); 

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer; 
}

VkShaderModule Helpers::CreateShaderModule(const VkDevice& device, char* const& pBlob, const size_t numBytes)
{
    VkShaderModule shaderModule = {};
    
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = numBytes;
    createInfo.pCode = reinterpret_cast<const uint32_t*>(pBlob);

    auto res = VK_CALL(vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule));
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Unable to create Shader Module");
    }

    return shaderModule;
}

void Helpers::DestroyShaderModule(const VkDevice& device, VkShaderModule& shaderModule)
{
    vkDestroyShaderModule(device, shaderModule, nullptr);
}

VkPipelineLayout Helpers::CreatePipelineLayout(const VkDevice& device)
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;


    VkPipelineLayout pipelineLayout = {};
    auto res = VK_CALL(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout));
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Unable to create Pipeline layout");
    }

    return pipelineLayout;
}

void Helpers::DestroyPipelineLayout(const VkDevice& device, VkPipelineLayout& layout)
{
    vkDestroyPipelineLayout(device, layout, nullptr);
}

VkRenderPass Helpers::CreateRenderPass(const VkDevice& device, const VkFormat& format)
{
    VkRenderPass pass = {};

    VkAttachmentDescription colourAttachment = {};
    colourAttachment.format = format;
    colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colourAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colourAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;


    VkAttachmentReference car = {};
    car.attachment = 0;
    car.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &car; 

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colourAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;


    auto res = VK_CALL(vkCreateRenderPass(device, &renderPassInfo, nullptr, &pass));
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to create Render Pass!\n");
    }

    return pass;
}

void Helpers::DestroyRenderPass(const VkDevice& device, VkRenderPass pass)
{
    vkDestroyRenderPass(device, pass, nullptr);
}

std::vector<VkFramebuffer> Helpers::CreateFrameBuffers(const VkDevice& device, const size_t count, VkExtent2D extents, const VkImageView* pViews, const VkRenderPass& pass)
{
    std::vector<VkFramebuffer> buffers(count);

    for (int i = 0; i < count; i++) {
        VkImageView attachments[] = {
            pViews[i]
        };

        VkFramebufferCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        createInfo.renderPass = pass;
        createInfo.attachmentCount = 1;
        createInfo.pAttachments = attachments;
        createInfo.width = extents.width;
        createInfo.height = extents.height;
        createInfo.layers = 1; 

        auto res = VK_CALL(vkCreateFramebuffer(device, &createInfo, nullptr, &buffers[i]));
        if (res != VK_SUCCESS) {
            throw std::runtime_error("Unable to create Framebuffer!");
        }
    }

    return buffers; 
}

void Helpers::DestroyFrameBuffer(const VkDevice& device, VkFramebuffer& buffer)
{
    vkDestroyFramebuffer(device, buffer, nullptr);
}

VkCommandPool Helpers::CreateCommandPool(const VkDevice& device, const size_t queueFamilyIdx)
{
    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO; 
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIdx;

    VkCommandPool pool = {};
    auto res = VK_CALL(vkCreateCommandPool(device, &poolInfo, nullptr, &pool));
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Unable to create Command Pool!");
    }
    return pool;
}

void Helpers::DestroyCommandPool(const VkDevice& device, VkCommandPool& pool)
{
    vkDestroyCommandPool(device, pool, nullptr);
}

VkCommandBuffer Helpers::CreateCommandBuffer(const VkDevice& device, const VkCommandPool& commandPool)
{
    VkCommandBufferAllocateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandPool = commandPool;
    info.commandBufferCount = 1;
    info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

    VkCommandBuffer buffer = {};
    auto res = VK_CALL(vkAllocateCommandBuffers(device, &info, &buffer));
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to allocate Command buffers!");
    }

    return buffer;
}

void Helpers::DestroyCommandBuffer(const VkDevice& device, const VkCommandPool& commandPool, const size_t count, VkCommandBuffer* buffers)
{
    vkFreeCommandBuffers(device, commandPool, count, buffers);
}

VkSemaphore Helpers::CreateSyncSemaphore(const VkDevice& device)
{
    VkSemaphoreCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkSemaphore semaphore;
    vkCreateSemaphore(device, &createInfo, nullptr, &semaphore);
    return semaphore;
}

void Helpers::DestroySyncSemaphore(const VkDevice& device, VkSemaphore& semaphore)
{
    vkDestroySemaphore(device, semaphore, nullptr);
}

VkFence Helpers::CreateSyncFence(const VkDevice& device)
{
    VkFenceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    createInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;    //Create the fence as Signalled, to avoid a deadlock. 

    VkFence fence;
    vkCreateFence(device, &createInfo, nullptr, &fence);
    return fence;
}

void Helpers::DestroySyncFence(const VkDevice& device, VkFence& fence)
{
    vkDestroyFence(device, fence, nullptr);
}
