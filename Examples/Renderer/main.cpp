//Vulkan Renderer
//Ewan Burnett 2023

#include "include/Utilities.h"
#include "include/Window.h"
#include "include/Renderer.h"
#include "include/GUI.h"
#include "include/Timer.h"

int main(){

    VKRenderer::Window::Init(); 

    //Create the Application Window
    VKRenderer::Window window;
    window.Create("Vulkan Renderer - Ewan Burnett (2023)", 600, 400);
    
    VKRenderer::Renderer renderer;
    renderer.Init(window); 

    VKRenderer::GUI gui;
    gui.Init(renderer);

    VKRenderer::Timer timer;
    timer.Reset();

    double deltaTime = 0.0;
    
    //Application Loop
    while(window.IsOpen()){
        //Poll any input events this frame.
        VKRenderer::Window::PollEvents(); 

        timer.Tick(); 
        deltaTime = timer.Get<std::chrono::milliseconds>().count() / 1000.0;
        Log::Print("DeltaTime: %f\r", deltaTime);
    }
    
    gui.Shutdown(); 
    renderer.Shutdown();
    VKRenderer::Window::Shutdown(); 
}
