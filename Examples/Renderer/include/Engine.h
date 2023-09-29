#ifndef _VKRENDERER_ENGINE_H
#define _VKRENDERER_ENGINE_H
#include <vulkan/vulkan.h>
#include "../include/Helpers.h"
#include "../include/Utilities.h"
#include "../include/Renderer.h"
#include <vector> 
namespace VKRenderer{
    class Engine{
        public:
            friend class Renderer;
            friend class GUI; 
            Engine();
            ~Engine(); 

            void Init(const Window& window);
            void Shutdown(); 

            void BeginFrame(VkClearValue clearColour); 
            void EndFrame(); 
        private:
      VkInstance m_Instance;
        VkPhysicalDevice m_PhysicalDevice; 
        VkDevice m_Device; 
        VkSurfaceKHR m_Surface;

        VkSwapchainKHR m_Swapchain;

        std::vector<VkImageView> m_ImageViews;
        VkCommandPool m_CommandPool; 

        std::vector<std::pair<size_t, VkQueue>> m_GraphicsQueues;
        std::vector<std::pair<size_t, VkQueue>> m_PresentationQueues;
        
        std::vector<VkImageView> m_SwapchainImages;
        VkSurfaceFormatKHR m_SwapchainFormat; 
        VkExtent2D m_SwapchainExtents;

        VkRenderPass m_RenderPass;
        VkPipeline m_Pipeline;
        std::vector<VkFramebuffer> m_FrameBuffers; 
        std::vector<VkShaderModule> m_ShaderModules;

        VkPipelineLayout m_PipelineLayout; 
        Helpers::QueueFamilyIndices m_QueueFamilyIndices; 

#ifdef DEBUG
        VkDebugUtilsMessengerEXT m_DebugLogger;
#endif

    };
}
#endif
