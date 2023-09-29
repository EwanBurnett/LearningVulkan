#ifndef _VKRENDERER_H
#define _VKRENDERER_H
#include "../include/Window.h"
#include "../include/Engine.h"
namespace VKRenderer{

    class Engine; 
    class Renderer{
        friend class GUI;

        public:
            Renderer();
            ~Renderer(); 

            void Init(const Window& window);
            void Shutdown();

            void Draw(); 
            void Clear(uint8_t r = 0x00, uint8_t g = 0x00, uint8_t b = 0x00, uint8_t a = 0xff);
            void Present(); 
        private:
            const Window* m_pWindow; 
            Engine* m_Engine; 

    };
}
#endif
