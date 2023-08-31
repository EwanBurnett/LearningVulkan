#include "Renderer.h"
#include <cstdio>

void Renderer::Init(GLFWwindow* window){
    printf("Initializing Renderer in window <0x%08x>\n\t-><0x%08x>\n", window, this);
}

void Renderer::Shutdown(){
    printf("Destroying Renderer <0x%08x>\n", this);
}
