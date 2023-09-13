#include <Utilities.h>
#include "Window.h"

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


    //Render Loop
    while(!window.ShouldClose())
    {
        //Poll Events
        glfwPollEvents(); 

        //Update

        //Render
    }
    
    window.Hide();  //Hide the window before cleaning up. 

    //Destroy the Renderer


    //Destroy the window
    window.Close(); 
    glfwTerminate();
}
