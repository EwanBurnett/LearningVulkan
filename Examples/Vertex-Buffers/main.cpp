#include <Utilities.h>
#include "Window.h"
#include "Renderer.h"
#include <chrono>

const char* windowTitle = "Vertex Buffers Demo";
const uint32_t defaultWidth = 600;
const uint32_t defaultHeight = 400;

int main()
{
    Log::Print("Vertex Buffers\n");
    
    //Load the mesh data

    //Create the Window
    glfwInit(); 

    Window window;
    window.Create(defaultWidth, defaultHeight, windowTitle);
    window.Show(); 

    //Create the Renderer
    Renderer renderer(window);
    renderer.InitData();

    float deltaTime = 0.0f;

    //Render Loop
    while(!window.ShouldClose())
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        //Poll Events
        glfwPollEvents(); 

        //Update

        //Render
        renderer.BeginFrame();
        renderer.Draw();
        renderer.EndFrame();

        auto endTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        //Poll Events
        printf("\rDelta Time: %05.6fms", deltaTime);
    }
    
    window.Hide();  //Hide the window before cleaning up. 

    //Destroy the Renderer


    //Destroy the window
    window.Close(); 
    glfwTerminate();
}
