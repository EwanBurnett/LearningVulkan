#ifndef _VKRENDERER_HELPERS_H
#define _VKRENDERER_HELPERS_H
#include <vulkan/vulkan.h>
#include <set>
#include <vector> 

//Wrapper for VkResult function calls. Dispatches calls to VkCall_Impl(), which reports any errors.
#define VK_CALL(function) Helpers::VkCall_Impl(function, __FILE__, __LINE__, __PRETTY_FUNCTION__)

class GLFWwindow; 

namespace VKRenderer {
    namespace Helpers {

        struct QueueFamilyIndices
        {
            std::set<std::pair<uint32_t, uint32_t>> Graphics; //Queue Family Index / Count 
            std::set<std::pair<uint32_t, uint32_t>> Presentation;
            std::set<std::pair<uint32_t, uint32_t>> Compute;
            std::set<std::pair<uint32_t, uint32_t>> Transfer;
            std::set<std::pair<uint32_t, uint32_t>> Sparse;
        };

        struct SwapChainSupportDetails {   //Could use a linked list for formats / presentModes, and retrieve Capabilities using a Helper? 
            VkSurfaceCapabilitiesKHR capabilities;
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentMode;
        };

        VkResult VkCall_Impl(const VkResult& result, const char* const file, const uint32_t line, const char* const function);

        /**
         * @brief Creates a Vulkan Instance handle.
         * @param appName The name of the application.
         * @param appVersion The packed version of the application (use VK_MAKE_API_VERSION)
         * @param pInstanceLayers a pointer to an array of strings, each containing a requested instance layer.
         * @param numInstanceLayers the number of instance layers requested.
         * @param pInstanceExtensions a pointer to an array of strings, each containing a requested instance extension.
         * @param numInstanceExtensions the number of instance extensions.
         * @param pAllocator A pointer to a VkAllocationCallbacks allocator, set to nullptr by default.
         * @return a VkInstance handle.
        */
        VkInstance CreateInstance(const char* const appName, const uint32_t appVersion, const char** const pInstanceLayers, const uint32_t numInstanceLayers, const char** const pInstanceExtensions, const uint32_t numInstanceExtensions, const VkAllocationCallbacks* pAllocator = nullptr);

        /**
         * @brief Creates a Vulkan Logical Device.
         * @param physicalDevice The Physical Device to Interface with.
         * @param pAllocator Optional pointer to a VkAllocationCallbacks allocator.
         * @return A VkDevice handle.
        */
        VkDevice CreateLogicalDevice(const VkPhysicalDevice& physicalDevice, const QueueFamilyIndices& queueFamilyIndices, const SwapChainSupportDetails& swapChainSupport, const char** const pDeviceExtensions = nullptr, const uint32_t numDeviceExtensions = 0, const VkAllocationCallbacks* pAllocator = nullptr);


        /**
         * @brief Retrieves a VkPhysicalDevice.
         * @param instance The Vulkan Instance.
         * @param pProperties Pointer to a VkPhysicalDeviceProperties struct containing desired properties.
         * @param pFeatures Pointer to a VkPhysicalDeviceFeatures struct containing desired features.
         * @return The first available Physical Device, or the Physical Device which best meets the property / feature requirements.
        */
        VkPhysicalDevice AcquirePhysicalDevice(const VkInstance& instance, const VkPhysicalDeviceProperties* pProperties = nullptr, const VkPhysicalDeviceFeatures* pFeatures = nullptr);

        /**
         * @brief Scores a Physical Device based on how closely it matches the requested property or feature requirements.
         * @param device The physical device to evaluate.
         * @param pProperties Optional pointer to a VkPhysicalDeviceProperties struct containing desired properties.
         * @param pFeatures Optional pointer to a VkPhysicalDeviceFeatures struct containing any desired features.
         * @return A score; Higher scores mean more features were supported.
        */
        uint32_t EvaluatePhysicalDeviceSuitability(const VkPhysicalDevice& device, const VkPhysicalDeviceProperties* pProperties = nullptr, const VkPhysicalDeviceFeatures* pFeatures = nullptr);


        //void RetrievePhysicalDeviceQueues(const VkPhysicalDevice& physicalDevice, VkQueue*& pQueues, uint32_t& numQueues, uint32_t queueCapabilities, uint32_t& queueFamilyIdx);

        //void GetPhysicalDeviceQueueFamilyProperties(const VkPhysicalDevice& physicalDevice,VkQueueFamilyProperties*& pProperties, uint32_t& propertyCount);

        QueueFamilyIndices GetQueueFamilyProperties(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR* pSurface = nullptr);

        /**
         * @brief Retrieves the Index of a Queue family which supports Graphics operations.
         * @param device The Physical device.
         * @return The index of a Graphics queue family, or -1 if none were found.
        */
        //uint32_t GetGraphicsQueueFamilyIndex(const VkPhysicalDevice& device);

        //VkQueue GetGraphicsQueue(const VkDevice& device, const VkPhysicalDevice& physicalDevice, const uint32_t queueIdx = 0);

        VkSurfaceKHR CreateWindowSurface(const VkInstance& instance, GLFWwindow* window, const VkAllocationCallbacks* pAllocator = nullptr);

        void DestroyWindowSurface(const VkInstance& instance, const VkSurfaceKHR& surface, const VkAllocationCallbacks* pAllocator = nullptr);


        /**
         * @brief Validates whether the current Vulkan Physical Device supports the specified extension.
         * @param extension String containing the name of the extension.
         * @param pExtensionProperties an optional array of VkExtensionProperties, containing previously evaluated supported extensions.
         * @param extensionPropertyCount optional number of elements within pExtensionProperties.
         * @return true if the extension is supported, false otherwise.
         * @remark If pExtensionProperties is nullptr, or extensionPropertyCount is 0, then this function will internally evaluate any supported extensions.
        */
        bool ValidateDeviceExtensionSupport(const VkPhysicalDevice& device, const char* const extension, VkExtensionProperties* pExtensionProperties = nullptr, const uint32_t extensionPropertyCount = 0);

        /**
         * @brief Validates an array of requested Physical Device Extensions.
         * @param pExtensions A pointer to an array of strings, representing the requested extensions.
         * @param numExtensions The number of requested extensions.
         * @return True if all requested extensions are supported. False otherwise.
        */
        bool ValidateArrayDeviceExtensionSupport(const VkPhysicalDevice& device, const char** const pExtensions, const uint32_t numExtensions);

        /**
         * @brief Validates whether the current Vulkan Instance supports the specified extension.
         * @param extension String containing the name of the extension.
         * @param pExtensionProperties an optional array of VkExtensionProperties, containing previously evaluated supported extensions.
         * @param extensionPropertyCount optional number of elements within pExtensionProperties.
         * @return true if the extension is supported, false otherwise.
         * @remark If pExtensionProperties is nullptr, or extensionPropertyCount is 0, then this function will internally evaluate any supported extensions.
        */
        bool ValidateInstanceExtensionSupport(const char* const extension, VkExtensionProperties* pExtensionProperties = nullptr, const uint32_t extensionPropertyCount = 0);

        /**
         * @brief Validates an array of requested Instance Extensions.
         * @param pExtensions A pointer to an array of strings, representing the requested extensions.
         * @param numExtensions The number of requested extensions.
         * @return True if all requested extensions are supported. False otherwise.
        */
        bool ValidateArrayInstanceExtensionSupport(const char** const pExtensions, const uint32_t numExtensions);

        /**
        * @brief Validates whether the current Vulkan Instance supports the specified layer.
        * @param layer String containing the name of the layer.
        * @param pLayerProperties an optional array of VkLayerProperties, containing previously evaluated supported layers.
        * @param layerPropertyCount optional number of elements within pLayerProperties.
        * @return true if the layer is supported, false otherwise.
        * @remark If pLayerProperties is nullptr, or layerPropertyCount is 0, then this function will internally evaluate any supported layers.
       */
        bool ValidateInstanceLayerSupport(const char* const layer, VkLayerProperties* pLayerProperties = nullptr, const uint32_t layerPropertyCount = 0);

        /**
         * @brief Validates an array of requested Instance Layers.
         * @param pLayers A pointer to an array of strings, representing the requested layers.
         * @param numLayers The number of requested layers.
         * @return True if all requested layers are supported. False otherwise.
        */
        bool ValidateArrayInstanceLayerSupport(const char** const pLayers, const uint32_t numLayers);

        /**
         * @brief Creates a VkDebugUtilsMessengerEXT object, used for Logging Debug messages.
         * @param instance the Vulkan instance.
         * @param pAllocator optional pointer to a VkAllocationCallbacks allocator.
         * @return a messenger object.
         * @remark Requires VK_LAYER_KHRONOS_validation.
        */
        VkDebugUtilsMessengerEXT CreateDebugLogger(const VkInstance& instance, const VkAllocationCallbacks* pAllocator = nullptr);

        /**
         * @brief Destroys an instance of VkDebugUtilsMessengerEXT, created via CreateDebugLogger().
         * @param instance the Vulkan instance.
         * @param messenger the messenger to destroy.
         * @param pAllocator optional pointer to a VkAllocationCallbacks allocator.
        */
        void DestroyDebugLogger(const VkInstance& instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks* pAllocator = nullptr);

        /**
         * @brief Logging Callback for VkDebugUtilsMessengerEXT
         * @param messageSeverity
         * @param messageType
         * @param pCallbackData
         * @param pUserData
         * @return
        */
        VKAPI_ATTR VkBool32 VKAPI_CALL DebugLog(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT messageType,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData
        );


        /**
         * @brief Destroys a Vulkan Instance handle.
         * @remark Must be called before the program exits.
        */
        void DestroyInstance(VkInstance& instance, const VkAllocationCallbacks* pAllocator = nullptr);

        /**
         * @brief Destroys a Vulkan Logical Device handle.
         * @param device The Logical Device to destroy
         * @param pAllocator Optional pointer to a VkAllocationCallbacks alloator.
        */
        void DestroyLogicalDevice(VkDevice& device, const VkAllocationCallbacks* pAllocator = nullptr);

        SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);

        VkSurfaceFormatKHR SelectSwapChainFormat(const uint32_t numFormats, const VkSurfaceFormatKHR* pFormats);

        VkPresentModeKHR SelectSwapChainPresentMode(const uint32_t numPresentModes, const VkPresentModeKHR* pModes);

        VkExtent2D SelectSwapChainExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);

        VkSwapchainKHR CreateSwapChain(const VkDevice& device, const VkSurfaceKHR& surface, const VkSurfaceFormatKHR& format, const VkPresentModeKHR& presentMode, const VkExtent2D& extents, const VkSurfaceCapabilitiesKHR capabilities, const VkAllocationCallbacks* pAllocator = nullptr);

        void DestroySwapChain(const VkDevice& device, VkSwapchainKHR& swapChain);

        std::vector<VkImageView> CreateImageViews(const VkDevice& device, const std::vector<VkImage>& swapChainImages, const VkFormat& swapChainImageFormat, const VkAllocationCallbacks* pAllocator = nullptr);

        void DestroyImageView(const VkDevice& device, VkImageView imageView, const VkAllocationCallbacks* pAllocator = nullptr);

        VkPipeline CreateGraphicsPipeline(const VkDevice& device, const VkShaderModule& vs, const VkShaderModule& fs, const VkExtent2D& swapchainExtents, const VkPipelineLayout& layout, const VkRenderPass& renderPass, const VkAllocationCallbacks* pAllocator = nullptr);
        void DestroyPipeline(const VkDevice& device, VkPipeline pipeline);

        std::vector<char> ReadShaderBlob(const char* fileName);

        VkShaderModule CreateShaderModule(const VkDevice& device, char* const& pBlob, const size_t numBytes);
        void DestroyShaderModule(const VkDevice& device, VkShaderModule& shaderModule);

        VkPipelineLayout CreatePipelineLayout(const VkDevice& device);
        void DestroyPipelineLayout(const VkDevice& device, VkPipelineLayout& layout);

        VkRenderPass CreateRenderPass(const VkDevice& device, const VkFormat& format);
        void DestroyRenderPass(const VkDevice& device, VkRenderPass pass);


        std::vector<VkFramebuffer> CreateFrameBuffers(const VkDevice& device, const size_t count, VkExtent2D extents, const VkImageView* pViews, const VkRenderPass& pass);
        void DestroyFrameBuffer(const VkDevice& device, VkFramebuffer& buffer);

        VkCommandPool CreateCommandPool(const VkDevice& device, const size_t queueFamilyIdx);
        void DestroyCommandPool(const VkDevice& device, VkCommandPool& pool);

        VkCommandBuffer CreateCommandBuffer(const VkDevice& device, const VkCommandPool& commanPool);
        void DestroyCommandBuffer(const VkDevice& device, const VkCommandPool& commandPool, const size_t count, VkCommandBuffer* buffers);

        VkSemaphore CreateSyncSemaphore(const VkDevice& device);
        void DestroySyncSemaphore(const VkDevice& device, VkSemaphore& semaphore);
        VkFence CreateSyncFence(const VkDevice& device);
        void DestroySyncFence(const VkDevice& device, VkFence& fence);

        VkBuffer CreateBuffer(const VkDevice& device, size_t size, VkBufferCreateFlags usageFlags, VkSharingMode sharingMode, const VkAllocationCallbacks* pAllocator = nullptr);
        void DestroyBuffer(const VkDevice& device, VkBuffer& buffer, const VkAllocationCallbacks* pAllocator = nullptr);
    }

}

#endif