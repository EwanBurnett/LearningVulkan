#ifndef _VKRENDERER_WINDOW_H
#define _VKRENDERER_WINDOW_H

#include <cstdint>
#include <string>

class GLFWwindow;

namespace VKRenderer{
    /* Represents a dow on the user's current platform.
     * A Wrapper around GLFW3, abstracting implementation details.
     * Before any functions can be called, Window::Init() MUST Be called. 
     * Additionally, Window::Shutdown() MUST be called before program termination. 
     * Windows can be instantiated by using Window::Create(), 
     * and may have a Title, Position, and size. 
     * Events may be bound to the window through Window::BindEvent()
     * but must be polled by a call to Window::PollEvents() every frame. 
     */
    class Window{
        public:
            Window();
            ~Window();
            
            static void Init(); 
            static void Shutdown(); 
    
            void Create(const std::string& Title, const uint16_t Width, const uint16_t Height);
            static void PollEvents();
            void BindEvent();

            bool IsOpen() const;
                        
        private:
            GLFWwindow* m_WindowHandle; 
            std::string m_Title;
            uint16_t m_Width;
            uint16_t m_Height; 

            int  m_PosX;
            int  m_PosY;
    };
}
#endif
