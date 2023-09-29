#include "../include/GUI.h"
#include <imgui.h>
#include <backends/imgui_impl_vulkan.h>
#include <backends/imgui_impl_glfw.h>

using namespace VKRenderer;

GUI::GUI(){

}

GUI::~GUI(){

}

void GUI::Init(Renderer& renderer, const Window& window){
    m_pRenderer = &renderer;
    Log::Message("[GUI]\tInitializing GUI within Renderer <0x%8d>\n", m_pRenderer);

    
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

        const auto* engine = m_pRenderer->m_Engine;
        VkDescriptorPool imguiPool;
        VkResult result = vkCreateDescriptorPool(engine->m_Device, &descriptorPoolInfo, nullptr, &imguiPool);

        //Initialize the ImGui library
        Log::Message("[GUI]\tCreating ImGui Context.\n");
        ImGui::CreateContext();

        ImGui_ImplGlfw_InitForVulkan(window.m_WindowHandle, true);

        ImGui_ImplVulkan_InitInfo initInfo = {};
        initInfo.Instance = engine->m_Instance;
        initInfo.PhysicalDevice = engine->m_PhysicalDevice;
        initInfo.Device = engine->m_Device;
        initInfo.Queue = engine->m_GraphicsQueues[0].second;
        //initInfo.DescriptorPool = engine->m_DescriptorPool;
        initInfo.MinImageCount = 3; 
        initInfo.ImageCount = 3; 
        initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT; 
    
        //ImGui_ImplVulkan_Init(&initInfo, engine->m_RenderPass);

        //ImGui_ImplVulkan_CreateFontsTexture(); 
    }
    
}

void GUI::Shutdown(){
    Log::Message("[GUI]\tShutting Down GUI.\n");
}


void GUI::Draw(){
    //ImGui::ShowDemoWindow();
}

