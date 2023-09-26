#include "../include/Renderer.h"
#include "../include/Window.h"
#include "../include/Utilities.h"
#include "../include/Engine.h"

using namespace VKRenderer;

Renderer::Renderer(){

}

Renderer::~Renderer(){

}

void Renderer::Init(const Window& window){
    m_pWindow = &window;

    Log::Message("[Renderer]\tInitializing Renderer within window <0x%8d>\n", m_pWindow->m_WindowHandle);

    m_Engine = new Engine(); 
    m_Engine->Init();    //Initialize the Vulkan Backend 
    //Initialize GUI using the Renderer's context. 
    //
}

void Renderer::Shutdown(){
    m_Engine->Shutdown(); 
    Log::Message("[Renderer]\tShutting Down Renderer.\n");
}
