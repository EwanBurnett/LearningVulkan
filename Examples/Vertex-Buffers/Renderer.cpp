#include "Window.h"
#include "Renderer.h"

#include <Utilities.h> 
#include <Helpers.h>
#include <vector> 
#include <stdexcept>

//Vulkan Renderer

static Helpers::QueueFamilyIndices g_QueueFamilyIndices = {}; //TODO: Figure out a better way of storing this. 

Renderer::Renderer(){

}


Renderer::Renderer(const Window& window){
    
    try{

        //Create the Vulkan Instance
        {
            //Evaluate any required extensions
            std::vector<const char*> instanceExtensions = {};
#ifdef DEBUG 
            instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

            //Retrieve GLFW's required extensions
            {
                uint32_t numExtensions = 0; 
                const char** glfwExtensions = nullptr;

                glfwExtensions = glfwGetRequiredInstanceExtensions(&numExtensions);
                for(uint32_t i = 0; i < numExtensions; i++){
                    instanceExtensions.push_back(glfwExtensions[i]);
                }

            }

            //Evaluate any required Layers
            std::vector<const char*> instanceLayers = {};
#ifdef DEBUG
            instanceLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

            //Create our Vulkan Handles
            m_Instance = Helpers::CreateInstance("VkDemo", 0, instanceLayers.data(), instanceLayers.size(), instanceExtensions.data(), instanceExtensions.size(), nullptr);
        }
#ifdef DEBUG
        //Create a Debug Logger 
        m_DebugLogger = Helpers::CreateDebugLogger(m_Instance);
#endif
        m_PhysicalDevice = Helpers::AcquirePhysicalDevice(m_Instance, nullptr, nullptr);

        m_Surface = Helpers::CreateWindowSurface(m_Instance, window.GLFWHandle(), nullptr);

        g_QueueFamilyIndices = Helpers::GetQueueFamilyProperties(m_PhysicalDevice, &m_Surface);

        auto supportDetails = Helpers::QuerySwapChainSupport(m_PhysicalDevice, m_Surface);
        
        //Create the Logical Device
        {
            std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

            m_Device = Helpers::CreateLogicalDevice(m_PhysicalDevice, g_QueueFamilyIndices, supportDetails, deviceExtensions.data(), deviceExtensions.size(), nullptr);
        }

        //Retrieve the Required Queues
        {
            for(auto& pair : g_QueueFamilyIndices.Graphics)
            {
                const size_t& index = pair.first;
                const size_t& count = pair.second;

                for(size_t i = 0; i < count; i++){
                    m_GraphicsQueues.push_back({});
                    m_GraphicsQueues.back().first = index;
                    vkGetDeviceQueue(m_Device, index, i, &m_GraphicsQueues.back().second);
                }
            }

            //TODO: Presentation Queues
        }
    
    }
    catch(std::exception& e){
        Log::Print(e.what());
    }

}


Renderer::~Renderer(){
    //Destroy any remaining handles
    Helpers::DestroyLogicalDevice(m_Device, nullptr);
    Helpers::DestroyWindowSurface(m_Instance, m_Surface, nullptr);
#ifdef DEBUG
    Helpers::DestroyDebugLogger(m_Instance, m_DebugLogger, nullptr);
#endif
    Helpers::DestroyInstance(m_Instance, nullptr);
}
