#include "Window.h"
#include "Vertex.h"
#include "Renderer.h"

#include <cstring>
#include <Utilities.h> 
#include <Helpers.h>
#include <vector> 
#include <stdexcept>

//Vertex Buffer Specific 
std::vector<Vertex> g_Vertices = {
    {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
};

VkBuffer g_VertexBuffer = {};
VkDeviceMemory g_DeviceMemory = {};


//Vulkan Renderer

static Helpers::QueueFamilyIndices g_QueueFamilyIndices = {}; //TODO: Figure out a better way of storing this. 

Renderer::Renderer(){

}


Renderer::Renderer(const Window& window){
    
    try{

        //Create the Vulkan Instance
        {
            //Evaluate any required extensions
            std::vector<const char*> instanceExtensions = {};
#ifdef DEBUG 
            instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif

            //Retrieve GLFW's required extensions
            {
                uint32_t numExtensions = 0; 
                const char** glfwExtensions = nullptr;

                glfwExtensions = glfwGetRequiredInstanceExtensions(&numExtensions);
                for(uint32_t i = 0; i < numExtensions; i++){
                    instanceExtensions.push_back(glfwExtensions[i]);
                }

            }

            //Evaluate any required Layers
            std::vector<const char*> instanceLayers = {};
#ifdef DEBUG
            instanceLayers.push_back("VK_LAYER_KHRONOS_validation");
#endif

            //Create our Vulkan Handles
            m_Instance = Helpers::CreateInstance("VkDemo", 0, instanceLayers.data(), instanceLayers.size(), instanceExtensions.data(), instanceExtensions.size(), nullptr);
        }
#ifdef DEBUG
        //Create a Debug Logger 
        m_DebugLogger = Helpers::CreateDebugLogger(m_Instance);
#endif
        m_PhysicalDevice = Helpers::AcquirePhysicalDevice(m_Instance, nullptr, nullptr);

        m_Surface = Helpers::CreateWindowSurface(m_Instance, window.GLFWHandle(), nullptr);

        g_QueueFamilyIndices = Helpers::GetQueueFamilyProperties(m_PhysicalDevice, &m_Surface);

        auto supportDetails = Helpers::QuerySwapChainSupport(m_PhysicalDevice, m_Surface);
        
        //Create the Logical Device
        {
            std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

            m_Device = Helpers::CreateLogicalDevice(m_PhysicalDevice, g_QueueFamilyIndices, supportDetails, deviceExtensions.data(), deviceExtensions.size(), nullptr);
        }

        //Retrieve the Required Queues
        {
            for(auto& pair : g_QueueFamilyIndices.Graphics)
            {
                const size_t& index = pair.first;
                const size_t& count = pair.second;

                for(size_t i = 0; i < count; i++){
                    m_GraphicsQueues.push_back({});
                    m_GraphicsQueues.back().first = index;
                    vkGetDeviceQueue(m_Device, index, i, &m_GraphicsQueues.back().second);
                }
            }

            //TODO: Presentation Queues
        }

        //Create the Swap Chain
        {
            VkSurfaceFormatKHR m_SwapchainFormat = Helpers::SelectSwapChainFormat(supportDetails.formats.size(), supportDetails.formats.data());
            VkPresentModeKHR presentMode = Helpers::SelectSwapChainPresentMode(supportDetails.presentMode.size(), supportDetails.presentMode.data());
            VkExtent2D m_SwapchainExtents = Helpers::SelectSwapChainExtent(supportDetails.capabilities, window.GLFWHandle());

            m_Swapchain = Helpers::CreateSwapChain(m_Device, m_Surface, m_SwapchainFormat, presentMode, m_SwapchainExtents, supportDetails.capabilities, nullptr); 

            {
                std::vector<VkImage> swapchainImages;
                uint32_t swapchainImageCount = 0;
                vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &swapchainImageCount, nullptr);
                swapchainImages.resize(swapchainImageCount);
                vkGetSwapchainImagesKHR(m_Device, m_Swapchain, &swapchainImageCount, swapchainImages.data());
                m_SwapchainImages = Helpers::CreateImageViews(m_Device, swapchainImages, m_SwapchainFormat.format, nullptr);
    
                m_RenderPass = Helpers::CreateRenderPass(m_Device, m_SwapchainFormat.format);
                m_FrameBuffers = Helpers::CreateFrameBuffers(m_Device, m_SwapchainImages.size(), m_SwapchainExtents, m_SwapchainImages.data(), m_RenderPass);
            }

        }

    }
    catch(std::exception& e){
        Log::Print(e.what());
    }

}


Renderer::~Renderer(){
    //Wait for the Device to be idle
    vkDeviceWaitIdle(m_Device);

    //Destroy any remaining handles

    vkDestroyBuffer(m_Device, g_VertexBuffer, nullptr);
    vkFreeMemory(m_Device, g_DeviceMemory, nullptr);
    for(auto& buffer : m_FrameBuffers){
        Helpers::DestroyFrameBuffer(m_Device, buffer);
    }

    Helpers::DestroyPipeline(m_Device, m_Pipeline);
    Helpers::DestroyRenderPass(m_Device, m_RenderPass);
    Helpers::DestroyPipelineLayout(m_Device, m_PipelineLayout);

    for(auto& shaderModule : m_ShaderModules){
        Helpers::DestroyShaderModule(m_Device, shaderModule);
    }

    for(auto& view : m_SwapchainImages){
        Helpers::DestroyImageView(m_Device, view);
    }

    Helpers::DestroySwapChain(m_Device, m_Swapchain);
    Helpers::DestroyLogicalDevice(m_Device, nullptr);
    Helpers::DestroyWindowSurface(m_Instance, m_Surface, nullptr);
#ifdef DEBUG
    Helpers::DestroyDebugLogger(m_Instance, m_DebugLogger, nullptr);
#endif
    Helpers::DestroyInstance(m_Instance, nullptr);
}


void Renderer::InitData(){
    //Create the Graphics Pipeline
    std::vector<char> vs_source = Helpers::ReadShaderBlob("vert.spv");
    std::vector<char> fs_source = Helpers::ReadShaderBlob("frag.spv");

    VkShaderModule vertexShaderModule = Helpers::CreateShaderModule(m_Device, vs_source.data(), vs_source.size()); 
    
    VkShaderModule fragmentShaderModule = Helpers::CreateShaderModule(m_Device, fs_source.data(), fs_source.size()); 

    m_ShaderModules.push_back(vertexShaderModule);
    m_ShaderModules.push_back(fragmentShaderModule);

    m_PipelineLayout = Helpers::CreatePipelineLayout(m_Device);

    //m_Pipeline = Helpers::CreateGraphicsPipeline(m_Device, vertexShaderModule, fragmentShaderModule, m_SwapchainExtents, m_PipelineLayout, m_RenderPass);
    //Create the Pipeline
    {
        VkPipelineDynamicStateCreateInfo dynamicState = {};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.pNext = nullptr;
        dynamicState.dynamicStateCount = 0; 
        dynamicState.pDynamicStates = nullptr; 

        VkPipelineShaderStageCreateInfo vsCreateInfo = {};
        vsCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vsCreateInfo.pNext = nullptr;
        vsCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT; 
        vsCreateInfo.module = vertexShaderModule; 
        vsCreateInfo.pName = "main";

 
        VkPipelineShaderStageCreateInfo fsCreateInfo = {};
        fsCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fsCreateInfo.pNext = nullptr;
        fsCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;  
        fsCreateInfo.module = fragmentShaderModule; 
        fsCreateInfo.pName = "main";

        VkPipelineShaderStageCreateInfo stages[] = {vsCreateInfo, fsCreateInfo};

        //Use our Vertex Attributes 

        auto bindingDescription = Vertex::GetBindingDescription(); 
        auto attributeDescriptions = Vertex::GetAttributeDescriptions(); 

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.pNext = nullptr;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();
        

        VkPipelineInputAssemblyStateCreateInfo sci = {};
        sci.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        sci.pNext = nullptr;
        sci.primitiveRestartEnable = VK_FALSE;
        sci.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

        VkViewport viewport = {};
        viewport.x = 0;
        viewport.y = 0;
        viewport.width = (float)m_SwapchainExtents.width;
        viewport.height = (float)m_SwapchainExtents.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = m_SwapchainExtents;

        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo RS = {};
        RS.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        RS.depthClampEnable = VK_FALSE;
        RS.rasterizerDiscardEnable = VK_FALSE;
        RS.polygonMode = VK_POLYGON_MODE_FILL;
        RS.cullMode = VK_CULL_MODE_BACK_BIT;
        RS.frontFace = VK_FRONT_FACE_CLOCKWISE;
        RS.depthBiasEnable = VK_FALSE;
        RS.depthBiasConstantFactor = 0.0f;
        RS.depthBiasClamp = 0.0f;
        RS.depthBiasSlopeFactor = 0.0f;
        RS.lineWidth = 1.0f;

        VkPipelineMultisampleStateCreateInfo msaa = {};
        msaa.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        msaa.sampleShadingEnable = VK_FALSE;
        msaa.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        msaa.minSampleShading = 1.0f;
        msaa.pSampleMask = nullptr;
        msaa.alphaToCoverageEnable = VK_FALSE;
        msaa.alphaToOneEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional


       
        VkGraphicsPipelineCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO; 
        createInfo.stageCount = 2;
        createInfo.pStages = stages;
        createInfo.pVertexInputState = &vertexInputInfo; 
        createInfo.pInputAssemblyState = &sci;
        createInfo.pViewportState = &viewportState;
        createInfo.pRasterizationState = &RS;
        createInfo.pMultisampleState = &msaa;
        createInfo.pDepthStencilState = nullptr;
        createInfo.pColorBlendState = &colorBlending;
        createInfo.pDynamicState = &dynamicState; 
        createInfo.layout = m_PipelineLayout;
        createInfo.renderPass = m_RenderPass;
        createInfo.subpass = 0;
        createInfo.basePipelineHandle = VK_NULL_HANDLE;
        createInfo.basePipelineIndex = -1;

        auto r = VK_CALL(vkCreateGraphicsPipelines(m_Device, VK_NULL_HANDLE, 1, &createInfo, nullptr, &m_Pipeline));
        if (r != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Graphics Pipeline!");
        }

    }


    //Create the Vertex Buffer
    {
        
        VkBufferCreateInfo bufferCreateInfo = {};
        bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferCreateInfo.pNext = nullptr;
        bufferCreateInfo.size = sizeof(g_Vertices[0]) * g_Vertices.size();
        bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT; 
        bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //Only one queue can use this buffer
        bufferCreateInfo.flags = 0; 

        if(vkCreateBuffer(m_Device, &bufferCreateInfo, nullptr, &g_VertexBuffer) != VK_SUCCESS){
            throw std::runtime_error("Failed to create Vertex Buffer!\n");
        }
   
        //Get the Memory Requirements of the Buffer, and allocate on the device
        {
            VkMemoryRequirements memRequirements = {};
            vkGetBufferMemoryRequirements(m_Device, g_VertexBuffer, &memRequirements);

            Log::Print("Vertex Buffer requires:\n\tSize: %d\n\tAlignment:%d\n\tMemory Type: %d\n", memRequirements.size, memRequirements.alignment, memRequirements.memoryTypeBits);

            VkPhysicalDeviceMemoryProperties memProperties = {};
            vkGetPhysicalDeviceMemoryProperties(m_PhysicalDevice, &memProperties);

            auto findMemoryType = [](VkPhysicalDeviceMemoryProperties& properties, VkMemoryPropertyFlags propertyFlags, uint32_t requirements) -> uint32_t{
                for(uint32_t i = 0; i < properties.memoryTypeCount; i++){
                    if((requirements & (1 << i)) && ((properties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags)){
                        return i;
                    } 
                }
                
                throw std::runtime_error("Failed to find a suitable memory type!\n");
                return -1; 
            };

            auto memIdx = findMemoryType(memProperties, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, memRequirements.memoryTypeBits);

            VkMemoryAllocateInfo allocInfo = {};
            allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO; 
            allocInfo.pNext = nullptr;
            allocInfo.allocationSize = memRequirements.size; 
            allocInfo.memoryTypeIndex = memIdx; 

            if(vkAllocateMemory(m_Device, &allocInfo, nullptr, &g_DeviceMemory) != VK_SUCCESS){
                throw std::runtime_error("Unable to allocate Device Memory!\n");
            }

            //Back the Vertex Buffer with memory
            vkBindBufferMemory(m_Device, g_VertexBuffer, g_DeviceMemory, 0);

            
            //Map the Vertex data into the device memory
            {
                void* pData = nullptr;
                vkMapMemory(m_Device, g_DeviceMemory, 0, bufferCreateInfo.size, 0, &pData);
                memcpy(pData, g_Vertices.data(), (size_t)bufferCreateInfo.size);
                vkUnmapMemory(m_Device, g_DeviceMemory);
            }




        }

    }

}


void Renderer::BeginFrame(){
    
}


void Renderer::EndFrame(){

}


void Renderer::Draw(){

}

