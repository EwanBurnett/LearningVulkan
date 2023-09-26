#ifndef _VKRENDERER_H
#define _VKRENDERER_H

namespace VKRenderer{

    class Engine; 
    class Window; 
    class Renderer{
        friend class GUI;

        public:
            Renderer();
            ~Renderer(); 

            void Init(const Window& window);
            void Shutdown();
        private:
            const Window* m_pWindow; 
            Engine* m_Engine; 

    };
}
#endif
