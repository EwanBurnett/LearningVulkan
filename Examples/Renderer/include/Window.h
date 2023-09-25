#ifndef _VKRENDERER_WINDOW_H
#define _VKRENDERER_WINDOW_H

#include <cstdint>

namespace VKRenderer{
    /* Represents a Window on the user's current platform.
     * A Wrapper around GLFW3, abstracting implementation details 
     * Windows can be instantiated by using Window::Create(), 
     * and may have a Title, Position, and size. 
     * Events may be bound to the window through Window::BindEvent()
     * but must be polled by a call to Window::PollEvents() every frame. 
     */
    class Window{
        public:

        private:
    };
}
#endif
