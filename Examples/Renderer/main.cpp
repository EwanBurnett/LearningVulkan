//Vulkan Renderer
//Ewan Burnett 2023

#include "include/Window.h"
#include "include/Renderer.h"
#include "include/GUI.h"

int main(){

    VKRenderer::Window::Init(); 

    //Create the Application Window
    VKRenderer::Window window;
    window.Create("Vulkan Renderer - Ewan Burnett (2023)", 600, 400);
    
    VKRenderer::Renderer renderer;
    renderer.Init(window); 

    VKRenderer::GUI gui;
    gui.Init(renderer);

    //Application Loop
    while(window.IsOpen()){
        //Poll any input events this frame.
        VKRenderer::Window::PollEvents(); 


    }
    
    gui.Shutdown(); 
    renderer.Shutdown();
    VKRenderer::Window::Shutdown(); 
}
