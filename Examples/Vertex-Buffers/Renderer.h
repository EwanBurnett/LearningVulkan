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


    private:
        VkInstance m_Instance;
        VkPhysicalDevice m_PhysicalDevice; 
        VkDevice m_Device; 
        VkSurfaceKHR m_Surface;

#ifdef DEBUG
        VkDebugUtilsMessengerEXT m_DebugLogger;
#endif

        std::vector<std::pair<size_t, VkQueue>> m_GraphicsQueues;
};

#endif
