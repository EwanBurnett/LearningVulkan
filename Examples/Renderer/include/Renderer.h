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
            void Present(); 
        private:
            const Window* m_pWindow; 
            Engine* m_Engine; 

    };
}
#endif
