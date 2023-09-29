#ifndef _VKRENDERER_GUI_H
#define _VKRENDERER_GUI_H

#include "../include/Window.h"
#include "../include/Utilities.h"
#include "../include/Engine.h"
#include "../include/Renderer.h"

namespace VKRenderer{
    class Window;
    class Renderer; 
    class GUI{
        public:
            GUI();
            ~GUI();

            void Init(Renderer& renderer, const Window& window);
            void Shutdown(); 
            void Draw();

        private:
            Renderer* m_pRenderer;
    };
}
#endif
