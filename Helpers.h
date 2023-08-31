#ifndef __HELPERS_H
#define __HELPERS_H

#include <vulkan/vulkan.h> 

namespace Helpers{

    //
    VkInstance CreateInstance();
    void DestroyInstance();

    VkDevice CreateDevice();
    void DestroyDevice(); 
}

#endif
