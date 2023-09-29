#include "../include/Renderer.h"
#include "../include/Utilities.h"

using namespace VKRenderer;

Renderer::Renderer(){

}

Renderer::~Renderer(){

}

void Renderer::Init(const Window& window){
    m_pWindow = &window;

    Log::Message("[Renderer]\tInitializing Renderer\n");

    m_Engine = new Engine(); 
    m_Engine->Init(window);    //Initialize the Vulkan Backend 
}

void Renderer::Shutdown(){
    m_Engine->Shutdown(); 
    Log::Message("[Renderer]\tShutting Down Renderer.\n");
}

void VKRenderer::Renderer::Draw()
{
    //Add geometry to a list, to be actually drawn when Present() is called. 
}

void VKRenderer::Renderer::Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    m_Engine->BeginFrame({r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f});
}

void VKRenderer::Renderer::Present()
{
    //Batch and draw any geometry 

    m_Engine->EndFrame(); 

}
