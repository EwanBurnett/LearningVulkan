#ifndef _VKDEMO_RENDERER_H
#define _VKDEMO_RENDERER_H

#include <vulkan/vulkan.h>
#include <vector>

class Window;

class Renderer
{
    public:
        Renderer();
        explicit Renderer(const Window& window);
        ~Renderer(); 

        void InitData(); //Temporary function, used to set up the demo.

        void BeginFrame();
        void EndFrame();
        void Draw();

    private:
        VkInstance m_Instance;
        VkPhysicalDevice m_PhysicalDevice; 
        VkDevice m_Device; 
        VkSurfaceKHR m_Surface;

        VkSwapchainKHR m_Swapchain;

        std::vector<VkImageView> m_ImageViews;

#ifdef DEBUG
        VkDebugUtilsMessengerEXT m_DebugLogger;
#endif

        std::vector<std::pair<size_t, VkQueue>> m_GraphicsQueues;
        
        std::vector<VkImageView> m_SwapchainImages;
        VkSurfaceFormatKHR m_SwapchainFormat; 
        VkExtent2D m_SwapchainExtents;

        VkRenderPass m_RenderPass;
        VkPipeline m_Pipeline;
        std::vector<VkFramebuffer> m_FrameBuffers; 
        std::vector<VkShaderModule> m_ShaderModules;

        VkPipelineLayout m_PipelineLayout; 
};

#endif
