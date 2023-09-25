#include "../include/GUI.h"
#include "../include/Utilities.h"
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

using namespace VKRenderer;

GUI::GUI(){

}

GUI::~GUI(){

}

void GUI::Init(Renderer& renderer){
    m_pRenderer = &renderer;
    Log::Message("[GUI]\tInitializing GUI within Renderer <0x%8d>\n", m_pRenderer);

    /*
    //Initialize ImGui
    {
        //Create a Descriptor Pool
        VkDescriptorPoolSize poolSizes[] = 
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 },
        };

        VkDescriptorPoolCreateInfo descriptorPoolInfo = {};
        descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        descriptorPoolInfo.pNext = nullptr;
        descriptorPoolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        descriptorPoolInfo.maxSets = 1000;
        descriptorPoolInfo.poolSizeCount = std::size(poolSizes);
        descriptorPoolInfo.pPoolSizes = poolSizes;

        VkDescriptorPool imguiPool;
        VkResult result = vkCreateDescriptorPool(m_pRenderer->m_Device, &descriptorPoolInfo, nullptr, &imguiPool);

        //Initialize the ImGui library
        ImGui::CreateContext();

        ImGui_ImplGlfw_InitForVulkan(m_pRenderer->m_pWindow->m_WindowHandle, true;
                ImGui_ImplVulkan_InitInfo initInfo = {};
                initInfo.Instance = m_pRenderer->m_Instance;
                initInfo.PhysicalDevice = m_pRenderer->m_PhysicalDevice;
                initInfo.Device = m_pRenderer->m_Device;
                initInfo.QueueFamily = m_pRenderer->m_QueueFamilyIndices.Graphics.first;
                initInfo.Queue = m_pRenderer->m_GraphicsQueues[0];
                initInfo.PipelineCache = m_pRenderer->m_PipelineCache;
                initInfo.DescriptorPool = m_pRenderer->m_DescriptorPool;
                //...
                //TODO: IMGUI Initialization with vulkan handles
                }
    */
}

void GUI::Shutdown(){
    Log::Message("Shutting Down GUI.\n");
}


void GUI::Draw(){
    //ImGui::ShowDemoWindow();
}

