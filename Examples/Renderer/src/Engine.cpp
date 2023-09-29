#include "../include/Engine.h"
#include <stdexcept> 
#include "../include/Window.h"

using namespace VKRenderer;

//Temporary Globals
VkCommandBuffer g_CommandBuffer = {};

VkSemaphore g_ImageAvailable = {};
VkSemaphore g_RenderFinished = {};
VkFence g_Fence = {};

uint32_t g_FrameIdx = 0; 


Engine::Engine(){
}

Engine::~Engine(){

}

void Engine::Init(const Window& window){
    Log::Message("[Engine]\tInitializing Vulkan Engine\n");
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

        m_Surface = Helpers::CreateWindowSurface(m_Instance, window.m_WindowHandle, nullptr);

        m_QueueFamilyIndices = Helpers::GetQueueFamilyProperties(m_PhysicalDevice, &m_Surface);

        auto supportDetails = Helpers::QuerySwapChainSupport(m_PhysicalDevice, m_Surface);
        
        //Create the Logical Device
        {
            std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

            m_Device = Helpers::CreateLogicalDevice(m_PhysicalDevice, m_QueueFamilyIndices, supportDetails, deviceExtensions.data(), deviceExtensions.size(), nullptr);
        }

        //Retrieve the Required Queues
        {
            for(auto& pair : m_QueueFamilyIndices.Graphics)
            {
                const size_t& index = pair.first;
                const size_t& count = pair.second;

                for(size_t i = 0; i < count; i++){
                    m_GraphicsQueues.push_back({});
                    m_GraphicsQueues.back().first = index;
                    vkGetDeviceQueue(m_Device, index, i, &m_GraphicsQueues.back().second);
                }
            }

            for(auto& pair : m_QueueFamilyIndices.Presentation)
            {
                const size_t& index = pair.first;
                const size_t& count = pair.second;

                for(size_t i = 0; i < count; i++){
                    m_PresentationQueues.push_back({});
                    m_PresentationQueues.back().first = index;
                    vkGetDeviceQueue(m_Device, index, i, &m_PresentationQueues.back().second);
                }
        }

        //Create the Swap Chain
        {
            VkSurfaceFormatKHR m_SwapchainFormat = Helpers::SelectSwapChainFormat(supportDetails.formats.size(), supportDetails.formats.data());
            VkPresentModeKHR presentMode = Helpers::SelectSwapChainPresentMode(supportDetails.presentMode.size(), supportDetails.presentMode.data());
            m_SwapchainExtents = Helpers::SelectSwapChainExtent(supportDetails.capabilities, window.m_WindowHandle);

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
    }
    catch(std::exception& e){
        Log::Print(e.what());
    }

    //Temp: Initialize Data
      m_CommandPool = Helpers::CreateCommandPool(m_Device, m_GraphicsQueues[0].first);
       g_CommandBuffer = Helpers::CreateCommandBuffer(m_Device, m_CommandPool); 

       g_Fence = Helpers::CreateSyncFence(m_Device);
       g_ImageAvailable = Helpers::CreateSyncSemaphore(m_Device);
       g_RenderFinished = Helpers::CreateSyncSemaphore(m_Device);

}

void Engine::Shutdown(){
    Log::Message("[Engine]\tShutting Down Vulkan Engine\n");
   //Wait for the Device to be idle
    vkDeviceWaitIdle(m_Device);

    //Destroy any remaining handles

    Helpers::DestroySyncSemaphore(m_Device, g_RenderFinished);
    Helpers::DestroySyncSemaphore(m_Device, g_ImageAvailable);
    Helpers::DestroySyncFence(m_Device, g_Fence);
    Helpers::DestroyCommandBuffer(m_Device, m_CommandPool, 1, &g_CommandBuffer);
    Helpers::DestroyCommandPool(m_Device, m_CommandPool);
    //vkDestroyBuffer(m_Device, g_IndexBuffer, nullptr);
    //vkFreeMemory(m_Device, g_IndexBufferMemory, nullptr);
    //vkDestroyBuffer(m_Device, g_VertexBuffer, nullptr);
    //vkFreeMemory(m_Device, g_DeviceMemory, nullptr);
    for(auto& buffer : m_FrameBuffers){
        Helpers::DestroyFrameBuffer(m_Device, buffer);
    }

    //Helpers::DestroyPipeline(m_Device, m_Pipeline);
    //Helpers::DestroyRenderPass(m_Device, m_RenderPass);
    //Helpers::DestroyPipelineLayout(m_Device, m_PipelineLayout);

    /*for(auto& shaderModule : m_ShaderModules){
        Helpers::DestroyShaderModule(m_Device, shaderModule);
    }*/

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

void VKRenderer::Engine::BeginFrame(VkClearValue clearColour)
{
    //Wait for the current frame
    vkWaitForFences(m_Device, 1, &g_Fence, VK_TRUE, UINT64_MAX);
    vkResetFences(m_Device, 1, &g_Fence);

    vkAcquireNextImageKHR(m_Device, m_Swapchain, UINT64_MAX, g_ImageAvailable, VK_NULL_HANDLE, &g_FrameIdx);
    vkResetCommandBuffer(g_CommandBuffer, 0);

    //Begin the Command Buffer
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pNext = nullptr;
    beginInfo.pInheritanceInfo = nullptr;
    beginInfo.flags = 0;

    vkBeginCommandBuffer(g_CommandBuffer, &beginInfo);

    VkRenderPassBeginInfo renderPassBeginInfo = {};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.pNext = nullptr;
    renderPassBeginInfo.renderPass = m_RenderPass;
    renderPassBeginInfo.framebuffer = m_FrameBuffers[g_FrameIdx];
    renderPassBeginInfo.renderArea.offset = {0, 0};
    renderPassBeginInfo.renderArea.extent = m_SwapchainExtents; 

    renderPassBeginInfo.clearValueCount = 1; 
    renderPassBeginInfo.pClearValues = &clearColour; 

    vkCmdBeginRenderPass(g_CommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

}

void VKRenderer::Engine::EndFrame()
{
    //End Command Buffer
    vkCmdEndRenderPass(g_CommandBuffer);
    vkEndCommandBuffer(g_CommandBuffer); 

    //Submit the recorded commands
    VkSemaphore waitFor[] = {g_ImageAvailable};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.pNext = nullptr;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitFor;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1; 
    submitInfo.pCommandBuffers = &g_CommandBuffer;
    
    VkSemaphore signalSemaphores[] = {g_RenderFinished};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if(vkQueueSubmit(m_GraphicsQueues[0].second, 1, &submitInfo, g_Fence) != VK_SUCCESS){
        throw std::runtime_error("Failed to submit command buffer!\n");
    }


    //Present 
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.pNext = nullptr;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores; 

    VkSwapchainKHR SwapChains[] = {m_Swapchain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = SwapChains;
    presentInfo.pImageIndices = &g_FrameIdx;
    presentInfo.pResults = nullptr;

    vkQueuePresentKHR(m_PresentationQueues[0].second, &presentInfo);

}
