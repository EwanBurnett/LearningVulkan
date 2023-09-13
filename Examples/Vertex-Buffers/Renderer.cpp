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

        //Create the Swap Chain
        {
            VkSurfaceFormatKHR m_SwapchainFormat = Helpers::SelectSwapChainFormat(supportDetails.formats.size(), supportDetails.formats.data());
            VkPresentModeKHR presentMode = Helpers::SelectSwapChainPresentMode(supportDetails.presentMode.size(), supportDetails.presentMode.data());
            VkExtent2D m_SwapchainExtents = Helpers::SelectSwapChainExtent(supportDetails.capabilities, window.GLFWHandle());

            m_Swapchain = Helpers::CreateSwapChain(m_Device, m_Surface, m_SwapchainFormat, presentMode, m_SwapchainExtents, supportDetails.capabilities, nullptr); 

            {
                std::vector<VkImage> swapchainImages;
                uint32_t swapchainImageCount = 0;
                vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &swapchainImageCount, nullptr);
                swapchainImages.resize(swapchainImageCount);
                vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &swapchainImageCount, swapchainImages.data());
                m_SwapchainImages = Helpers::CreateImageViews(m_Device, swapchainImages, m_SwapchainFormat.format, nullptr);
    
                m_RenderPass = Helpers::CreateRenderPass(m_Device, m_SwapchainFormat.format);
                m_FrameBuffers = Helpers::CreateFrameBuffers(m_Device, m_SwapchainImages.size(), m_SwapchainExtents, m_SwapchainImages.data(), m_RenderPass);
            }

        }

    }
    catch(std::exception& e){
        Log::Print(e.what());
    }

}


Renderer::~Renderer(){
    //Wait for the Device to be idle
    vkDeviceWaitIdle(m_Device);

    //Destroy any remaining handles

    for(auto& buffer : m_FrameBuffers){
        Helpers::DestroyFrameBuffer(m_Device, buffer);
    }

    Helpers::DestroyPipeline(m_Device, m_Pipeline);
    Helpers::DestroyRenderPass(m_Device, m_RenderPass);
    Helpers::DestroyPipelineLayout(m_Device, m_PipelineLayout);

    for(auto& shaderModule : m_ShaderModules){
        Helpers::DestroyShaderModule(m_Device, shaderModule);
    }

    for(auto& view : m_SwapchainImages){
        Helpers::DestroyImageView(m_Device, view);
    }

    Helpers::DestroySwapChain(m_Device, m_Swapchain);
    Helpers::DestroyLogicalDevice(m_Device, nullptr);
    Helpers::DestroyWindowSurface(m_Instance, m_Surface, nullptr);
#ifdef DEBUG
    Helpers::DestroyDebugLogger(m_Instance, m_DebugLogger, nullptr);
#endif
    Helpers::DestroyInstance(m_Instance, nullptr);
}


void Renderer::InitData(){
    //Create the Graphics Pipeline
    std::vector<char> vs_source = Helpers::ReadShaderBlob("vert.spv");
    std::vector<char> fs_source = Helpers::ReadShaderBlob("frag.spv");

    VkShaderModule vertexShaderModule = Helpers::CreateShaderModule(m_Device, vs_source.data(), vs_source.size()); 
    
    VkShaderModule fragmentShaderModule = Helpers::CreateShaderModule(m_Device, fs_source.data(), fs_source.size()); 

    m_ShaderModules.push_back(vertexShaderModule);
    m_ShaderModules.push_back(fragmentShaderModule);

    m_PipelineLayout = Helpers::CreatePipelineLayout(m_Device);

    m_Pipeline = Helpers::CreateGraphicsPipeline(m_Device, vertexShaderModule, fragmentShaderModule, m_SwapchainExtents, m_PipelineLayout, m_RenderPass);

}


void Renderer::BeginFrame(){

}


void Renderer::EndFrame(){

}


void Renderer::Draw(){

}

