#ifndef _VKRENDERER_TIMER_H
#define _VKRENDERER_TIMER_H

#include <chrono>

namespace VKRenderer{

    //Timing Utility
    //A Lightweight class representing a Start and End time. 
    //Advance the timer by calling Tick().
    class Timer{
        public: 
            Timer();
            ~Timer();

            void Reset(); 
            void Tick();
            template <typename T>
            T Get() const;
        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> m_PreviousTime;
            std::chrono::time_point<std::chrono::high_resolution_clock> m_CurrentTime;
    };

    inline Timer::Timer(){
        Reset();
    }

    inline Timer::~Timer(){

    }

    inline void Timer::Reset(){
        m_PreviousTime = std::chrono::high_resolution_clock::now();
        m_CurrentTime = std::chrono::high_resolution_clock::now();
    }

    inline void Timer::Tick(){
        m_PreviousTime = m_CurrentTime;
        m_CurrentTime = std::chrono::high_resolution_clock::now();
    }

    template <typename T>
    inline T Timer::Get() const{
            auto duration = m_CurrentTime - m_PreviousTime;
            return std::chrono::duration_cast<T>(duration); 
        } 
}


#endif
