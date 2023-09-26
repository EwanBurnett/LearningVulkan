#ifndef _VKRENDERER_ENGINE_H
#define _VKRENDERER_ENGINE_H
#include <vulkan/vulkan.h>

namespace VKRenderer{
    class Engine{
        public:
            friend class Renderer;
            Engine();
            ~Engine(); 

            void Init();
            void Shutdown(); 

        private:
            VkInstance m_Instance;
            VkPhysicalDevice m_PhysicalDevice; 
            VkDevice m_Device; 
    };
}
#endif
