#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <GLFW/glfw3.h>
#include "Renderer.h"

int main(int argc, const char** argv)
{
    printf("Vulkan 3D Rendering Demo\nEwan Burnett - 2023\n");
    
    int width = 600;
    int height = 400; 
    const char* title = "Vulkan Rendering Demo";

    //Parse the CMD arguments
    if(argc <= 1)
    {
        printf("USAGE:\n\t-w -width : specifies the width of the window (600px by default)\n\t-h -height : specifies the height of the window (400px by default)\n");
    }
    else //Otherwise, the user has specified one or more arguments. so parse them. 
    {
        for(int i = 1; i < argc; i++){
            if(strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "-width") == 0){
                width = std::atoi(argv[i + 1]);
                i++;
            }
            else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-height") == 0){
                height = std::atoi(argv[i + 1]);
                i++;
            }
        }    
    }

    //Create a window using GLFW
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        
    printf("Creating window %s\n\tWidth: %d\n\tHeight: %d\n", title, width, height);

    //Init the Renderer
    Renderer renderer; 
    renderer.Init(window);

    //Main Loop
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents(); 

    }

    glfwDestroyWindow(window);
    renderer.Shutdown();

}
