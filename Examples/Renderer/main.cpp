//Vulkan Renderer
//Ewan Burnett 2023

#include "include/Window.h"

int main(){

    VKRenderer::Window::Init(); 

    //Create thw Application Window
    VKRenderer::Window window;
    window.Create("Vulkan Renderer - Ewan Burnett (2023)", 600, 400);
    


    //Application Loop
    while(window.IsOpen()){
        //Poll any input events this frame.
        VKRenderer::Window::PollEvents(); 


    }
    
    VKRenderer::Window::Shutdown(); 
}
