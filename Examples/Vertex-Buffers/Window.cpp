#include "Window.h"
#include <Utilities.h>

//GLFW Window Wrapper

Window::Window(){
    m_Width = 0;
    m_Height = 0;
    m_Position_X = 0;
    m_Position_Y = 0;

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

}


Window::Window(uint32_t width, uint32_t height, const std::string& title){
    m_Title = title;

    m_Width = width;
    m_Height = height; 

    m_Position_X = 0; 
    m_Position_Y = 0; 
}


Window::~Window(){

}


GLFWwindow* Window::GLFWHandle() const{
    return m_Window; 
}

       
void Window::BindFunction(std::function<void(GLFWwindow*, int, int, int, int)> callback){
    //glfwSetKeyCallback(m_Window, callback); //TODO: Implement
}


void Window::Resize(uint32_t width, uint32_t height){
    //TODO: Implement
    m_Width = width;
    m_Height = height; 
}

void Window::SetPosition(int x, int y){
    m_Position_X = x;
    m_Position_Y = y;

    glfwSetWindowPos(m_Window, m_Position_X, m_Position_Y);
}


bool Window::ShouldClose() const{
    return glfwWindowShouldClose(m_Window);
}

void Window::Create() 
{
    m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
    Hide();
}


void Window::Create(uint32_t width, uint32_t height, const std::string& title){
    m_Width = width;
    m_Height = height;
    m_Title = title;

    Create(); 
}


void Window::Close(){
    glfwSetWindowShouldClose(m_Window, true);
}


void Window::Destroy(){
    glfwDestroyWindow(m_Window);
}


void Window::Show(){
    glfwShowWindow(m_Window);
}

void Window::Hide(){
    glfwHideWindow(m_Window);
}





