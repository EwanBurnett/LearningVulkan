#ifndef _VKDEMO_VERTEX_H
#define _VKDEMO_VERTEX_H

#include <vulkan/vulkan.h>
#include <glm/glm.hpp> //Use GLM for simplicity
#include <array> 

struct Vertex{
    glm::vec3 Position; 
    glm::vec4 Colour; 


    static VkVertexInputBindingDescription GetBindingDescription(){
        VkVertexInputBindingDescription desc = {};
        desc.binding = 0; //Bind to input slot 0
        desc.stride = sizeof(Vertex);   //How many bytes per element 
        desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return desc;
    } 


    //TODO: Use std::vector instead? 
    static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions(){
        std::array<VkVertexInputAttributeDescription, 2> descriptions = {};
        descriptions[0].binding = 0;
        descriptions[0].location = 0; 
        descriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        descriptions[0].offset = offsetof(Vertex, Position);

        descriptions[1].binding = 0;
        descriptions[1].location = 1; 
        descriptions[1].format = VK_FORMAT_R32G32B32A32_SFLOAT;
        descriptions[1].offset = offsetof(Vertex, Colour);

        return descriptions;
    }
};

#endif
