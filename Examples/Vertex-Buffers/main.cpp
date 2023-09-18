#include <Utilities.h>
#include "Window.h"
#include "Renderer.h"
#include <chrono>
#include <glm/gtc/matrix_transform.hpp>

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

    float deltaTime = 0.0;
    uint64_t frameCount = 0;

    //Render Loop
    while(!window.ShouldClose())
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        //Poll Events
        glfwPollEvents(); 

        //Update

        glm::vec3 camPos = {0.0f, 0.0f, -3.0f};
        glm::mat4x4 view = glm::translate(glm::mat4x4(1.0f), camPos);
        glm::mat4x4 projection = glm::perspective(glm::radians(70.0f), 1700.0f / 900.0f, 0.1f, 200.0f);
        glm::mat4x4 world = glm::rotate(glm::mat4x4{1.0f}, glm::radians(frameCount * 0.004f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4x4 pvw = projection * view * world;

        //Render
        renderer.BeginFrame();
        renderer.Draw(pvw);
        renderer.EndFrame();

        auto endTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        frameCount++;
        //Poll Events
        printf("\rDelta Time: %05.6fms", deltaTime);
    }
    
    window.Hide();  //Hide the window before cleaning up. 

    //Destroy the Renderer


    //Destroy the window
    window.Close(); 
    glfwTerminate();
}
