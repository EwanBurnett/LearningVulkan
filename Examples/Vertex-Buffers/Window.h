#ifndef _VKDEMO_WINDOW_H
#define _VKDEMO_WINDOW_H

#include <string>
#include <cstdint>
#include <functional>

#include <GLFW/glfw3.h> 

class Window
{
    public:
        Window();
        Window(uint32_t width, uint32_t height, const std::string& title); 
        ~Window();
        
        GLFWwindow* GLFWHandle() const; 
        void BindFunction(std::function<void(GLFWwindow*, int, int, int, int)> callback);

        void Resize(uint32_t width, uint32_t height);
        void SetPosition(int x, int y);

        bool ShouldClose() const;

        void Create();
        void Create(uint32_t width, uint32_t height, const std::string& title);
        void Close(); 
        void Destroy(); 

        void Show();
        void Hide(); 

    private:
        GLFWwindow* m_Window;

        std::string m_Title;
        uint32_t m_Width;    //TODO: Replace with Vector2i <uint16_t>
        uint32_t m_Height;

        int m_Position_X;    //TODO: Replace with Vector2i <int>
        int m_Position_Y; 
};

#endif
