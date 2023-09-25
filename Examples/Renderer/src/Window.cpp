#include "../include/Window.h"
#include "../include/Utilities.h"

#include <GLFW/glfw3.h>

using namespace VKRenderer;

const char* DEFAULT_TITLE = "VKRenderer Window";
const uint16_t DEFAULT_WIDTH = 600u;
const uint16_t DEFAULT_HEIGHT = 400u;

Window::Window(){
    m_Title = DEFAULT_TITLE;
    m_Width = DEFAULT_WIDTH;
    m_Height = DEFAULT_HEIGHT;

    m_WindowHandle = nullptr;
    m_PosX = 0;
    m_PosY = 0;
}

Window::~Window(){

}

void Window::Init(){
    Log::Message("[Window]\tInitializing GLFW.\n");
    int result = glfwInit(); //Initialize GLFW. 
    if(result != GLFW_TRUE){
        Log::Fatal(__FILE__, __LINE__, __PRETTY_FUNCTION__, true, "[Window]\tFailed to Initialize GLFW With code %d!", result); 
    }

}

void Window::Shutdown(){
    Log::Message("[Window]\tShutting Down GLFW.\n");
    glfwTerminate(); 
}


void Window::Create(const std::string& Title, const uint16_t Width, const uint16_t Height){

    //Validate Window Creation Parameters
    {
        if(Title.empty()){
            Log::Warning("[Window]\tTitle was Empty! Window Title must be a valid String. Defaulting to %s.\n", DEFAULT_TITLE);
        }
        else{
            m_Title = Title;
        }
        if(Width == 0){
            Log::Warning("[Window]\tWidth was 0! Window Width must be > 0. Defaulting to %d.\n", DEFAULT_WIDTH);
        }
        else{
            m_Width = Width;
        }
        if(Height == 0){
            Log::Warning("[Window]\tHeight was 0! Window Height must be > 0. Defaulting to %d.\n", DEFAULT_HEIGHT);
        }
        else{
            m_Height = Height;
        }
    }
    Log::Message("[Window]\tCreating Window %s (%dx%d)\n", m_Title.c_str(), m_Width, m_Height);
    m_WindowHandle = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

    if(!m_WindowHandle){
        Log::Fatal(__FILE__, __LINE__, __PRETTY_FUNCTION__, true, "[Window]\tWindow Creation Failed!\nm_WindowHandle was nullptr!\n");
    }
}


void Window::BindEvent(){

}

void Window::PollEvents(){
    glfwPollEvents();

}


bool Window::IsOpen() const {
    if(m_WindowHandle){
        return !glfwWindowShouldClose(static_cast<GLFWwindow*>(m_WindowHandle));
    }
    else{
        Log::Error(__FILE__, __LINE__, __PRETTY_FUNCTION__, "[Window]\tWindow::IsOpen() was polled, but m_WindowHandle was nullptr!\nDid you call Window::Create()?\nReturning False by default.\n");
        return false; 
    }
}
