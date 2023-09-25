#ifndef _VKRENDERER_GUI_H
#define _VKRENDERER_GUI_H

namespace VKRenderer{
    class Renderer; 
    class GUI{
        public:
            GUI();
            ~GUI();

            void Init(Renderer& renderer);
            void Shutdown(); 
            void Draw();

        private:
            Renderer* m_pRenderer;
    };
}
#endif
