//Vulkan Renderer
//Ewan Burnett 2023

#include "include/Utilities.h"
#include "include/Window.h"
#include "include/Renderer.h"
#include "include/GUI.h"
#include "include/Timer.h"

#include <thread>
int main(){

    VKRenderer::Window::Init(); 

    //Create the Application Window
    VKRenderer::Window window;
    window.Create("Vulkan Renderer - Ewan Burnett (2023)", 600, 400);
    
    VKRenderer::Renderer renderer;
    renderer.Init(window); 

    VKRenderer::GUI gui;
    gui.Init(renderer, window);

    VKRenderer::Timer timer;
    timer.Reset();

    double deltaTime = 0.0;
    
    //Application Loop
    while (window.IsOpen()) {
        //Poll any input events this frame.
        VKRenderer::Window::PollEvents();

        //Update
        {
            timer.Tick();
            deltaTime = (timer.Get<std::chrono::nanoseconds>().count() / 1'000'000'000.0);    //Very High-resolution Delta Time 
            uint64_t fps = 1.0 / deltaTime;
            Log::Print("DeltaTime: %f\t%04u fps\r", deltaTime, fps);
        }

        //Render
        renderer.Clear();

        gui.Draw(); 

        renderer.Present(); 
    }
    
    gui.Shutdown(); 
    renderer.Shutdown();
    VKRenderer::Window::Shutdown(); 
}
