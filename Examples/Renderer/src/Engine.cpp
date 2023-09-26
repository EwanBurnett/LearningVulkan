#include "../include/Engine.h"
#include "../include/Utilities.h"

using namespace VKRenderer;

Engine::Engine(){

}

Engine::~Engine(){

}

void Engine::Init(){
    Log::Message("[Engine]\tInitializing Vulkan Engine\n");
}

void Engine::Shutdown(){
    Log::Message("[Engine]\tShutting Down Vulkan Engine\n");
}
