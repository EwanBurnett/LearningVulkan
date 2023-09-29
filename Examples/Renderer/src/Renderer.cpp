#include "../include/Renderer.h"
#include "../include/Utilities.h"

using namespace VKRenderer;

Renderer::Renderer(){

}

Renderer::~Renderer(){

}

void Renderer::Init(const Window& window){
    m_pWindow = &window;

    Log::Message("[Renderer]\tInitializing Renderer within window <0x%8d>\n", m_pWindow->m_WindowHandle);

    m_Engine = new Engine(); 
    m_Engine->Init(window);    //Initialize the Vulkan Backend 
    //Initialize GUI using the Renderer's context. 
    //
}

void Renderer::Shutdown(){
    m_Engine->Shutdown(); 
    Log::Message("[Renderer]\tShutting Down Renderer.\n");
}

void VKRenderer::Renderer::Draw()
{
    //Add geometry to a list, to be actually drawn when Present() is called. 
}

void VKRenderer::Renderer::Present()
{
    m_Engine->BeginFrame(); 

    //Batch and draw any geometry 

    m_Engine->EndFrame(); 

}
