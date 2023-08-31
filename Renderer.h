#ifndef __RENDERER_H
#define __RENDERER_H
#include <GLFW/glfw3.h>

struct Renderer{
    void Init(GLFWwindow* window);

    void Shutdown();

private:
    GLFWwindow* m_pWindow;
};
#endif

