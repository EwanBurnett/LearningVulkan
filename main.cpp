#include <cstdio>
#include <exception>
#include <vector>
#include <map>

#include "Helpers.h"


void RecordCommandBuffer(VkCommandBuffer buffer, uint32_t imgIdx, const VkRenderPass& pass) {
     
}


int main()
{
    try {
        glfwInit(); 
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        auto window = glfwCreateWindow(800, 600, "Hello Triangle", nullptr, nullptr);

        glfwHideWindow(window);
        
        std::vector<const char*> requestedExtensions = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };
    
        //Get the GLFW required extensions
        {
            uint32_t glfwNumExt = 0;
            const char** glfwExt = nullptr; 

            glfwExt = glfwGetRequiredInstanceExtensions(&glfwNumExt);

            for (int i = 0; i < glfwNumExt; i++) {
                requestedExtensions.push_back(glfwExt[i]);
            }
        }

        bool extensionsValid = Helpers::ValidateArrayInstanceExtensionSupport(requestedExtensions.data(), requestedExtensions.size());
        
        std::vector<const char*> requestedLayers = { "VK_LAYER_KHRONOS_validation" };
        //bool layersValid = Helpers::ValidateArrayInstanceLayerSupport(requestedLayers.data(), requestedLayers.size());

        auto instance = Helpers::CreateInstance("Hello-Triangle", VK_MAKE_API_VERSION(0, 1, 0, 0), requestedLayers.data(), requestedLayers.size(), requestedExtensions.data(), requestedExtensions.size());

        auto debugLogger = Helpers::CreateDebugLogger(instance);

        auto physicalDevice = Helpers::AcquirePhysicalDevice(instance); 
        auto surface = Helpers::CreateWindowSurface(instance, window, nullptr); //Create this before the logical device
       
        std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
        auto qfi = Helpers::GetQueueFamilyProperties(physicalDevice, &surface);

        auto swapchainSupportDetails = Helpers::QuerySwapChainSupport(physicalDevice, surface);
        auto device = Helpers::CreateLogicalDevice(physicalDevice, qfi, swapchainSupportDetails, deviceExtensions.data(), deviceExtensions.size());

        std::vector<std::pair<size_t, VkQueue>> GraphicsQueues;
        for (auto& pair : qfi.Graphics) {
            const auto& idx = pair.first;
            const auto& count = pair.second; 

            for (int i = 0; i < count; i++) {
                GraphicsQueues.push_back({});
                GraphicsQueues.back().first = idx;
                vkGetDeviceQueue(device, idx, i, &GraphicsQueues.back().second);
            }
        }

        std::vector<VkQueue> PresentationQueues;
        for (auto& pair : qfi.Presentation) {
            const auto& idx = pair.first;
            const auto& count = pair.second;

            for (int i = 0; i < count; i++) {
                PresentationQueues.push_back({});

                vkGetDeviceQueue(device, idx, i, &PresentationQueues.back());
            }
        }

        //Create the SwapChain
        auto swapChainFormat = Helpers::SelectSwapChainFormat(swapchainSupportDetails.formats.size(), swapchainSupportDetails.formats.data());
        
        auto swapChainPresentMode = Helpers::SelectSwapChainPresentMode(swapchainSupportDetails.presentMode.size(), swapchainSupportDetails.presentMode.data());
        auto swapChainExtents = Helpers::SelectSwapChainExtent(swapchainSupportDetails.capabilities, window);

        auto SwapChain = Helpers::CreateSwapChain(device, surface, swapChainFormat, swapChainPresentMode, swapChainExtents, swapchainSupportDetails.capabilities);

        //This really should be done in a helper, but I don't want to return a std::vector...
        std::vector<VkImage> swapChainImages;
        {
            uint32_t imageCount = 0; 
            vkGetSwapchainImagesKHR(device, SwapChain, &imageCount, nullptr);
            swapChainImages.resize(imageCount);
            vkGetSwapchainImagesKHR(device, SwapChain, &imageCount, swapChainImages.data());
        }
        

        auto imageViews = Helpers::CreateImageViews(device, swapChainImages, swapChainFormat.format);

        //Create the Graphics Pipeline
        
        auto vertexShader = Helpers::ReadShaderBlob("vert.spv");
        auto fragmentShader = Helpers::ReadShaderBlob("frag.spv");

        auto VS = Helpers::CreateShaderModule(device, vertexShader.data(), vertexShader.size());
        auto FS = Helpers::CreateShaderModule(device, fragmentShader.data(), fragmentShader.size());
        auto pipelineLayout = Helpers::CreatePipelineLayout(device); 
        auto renderPass = Helpers::CreateRenderPass(device, swapChainFormat.format); 
        auto pipeline = Helpers::CreateGraphicsPipeline(device, VS, FS, swapChainExtents, pipelineLayout, renderPass);
        
        auto frameBuffers = Helpers::CreateFrameBuffers(device, imageViews.size(), swapChainExtents, imageViews.data(), renderPass);

        auto commandPool = Helpers::CreateCommandPool(device, GraphicsQueues[0].first);
        auto commandBuffer = Helpers::CreateCommandBuffer(device, commandPool);


        //Draw the triangle!!!!
        auto recordCommandBuffer = [&](uint32_t idx) {
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = 0;
            beginInfo.pInheritanceInfo = nullptr;

            if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
                throw "Unable to record command buffer";
            }

            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderPass;
            renderPassInfo.framebuffer = frameBuffers[idx];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = swapChainExtents;

            VkClearValue clearColour = { {{0.0f, 0.0f, 0.0f, 1.0f}} };  //Clear to black
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColour;

            vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
            vkCmdDraw(commandBuffer, 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffer);
            if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
                throw "Failed to record Commands!";
            }
        };

        VkSemaphore S_ImageAvailable = Helpers::CreateSyncSemaphore(device);
        VkSemaphore S_RenderFinished = Helpers::CreateSyncSemaphore(device);
        VkFence F_InFlight = Helpers::CreateSyncFence(device);


        
        glfwShowWindow(window);

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            vkWaitForFences(device, 1, &F_InFlight, VK_TRUE, UINT64_MAX);
            vkResetFences(device, 1, &F_InFlight);

            uint32_t imageIdx;
            vkAcquireNextImageKHR(device, SwapChain, UINT64_MAX, S_ImageAvailable, VK_NULL_HANDLE, &imageIdx); 
            vkResetCommandBuffer(commandBuffer, 0);

            recordCommandBuffer(imageIdx);

            VkSubmitInfo submitInfo = {};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

            VkSemaphore waitFor[] = { S_ImageAvailable };
            VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
            submitInfo.waitSemaphoreCount = 1;
            submitInfo.pWaitSemaphores = waitFor;
            submitInfo.pWaitDstStageMask = waitStages;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            VkSemaphore signalSemaphores[] = { S_RenderFinished };
            submitInfo.signalSemaphoreCount = 1;
            submitInfo.pSignalSemaphores = signalSemaphores;

            if (vkQueueSubmit(GraphicsQueues[0].second, 1, &submitInfo, F_InFlight) != VK_SUCCESS) {
                throw "Failed to submit command buffer!";
            }

           
            VkPresentInfoKHR presentInfo = {};
            presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            presentInfo.waitSemaphoreCount = 1;
            presentInfo.pWaitSemaphores = signalSemaphores;

            VkSwapchainKHR SwapChains[] = { SwapChain };
            presentInfo.swapchainCount = 1;
            presentInfo.pSwapchains = SwapChains;
            presentInfo.pImageIndices = &imageIdx;
            presentInfo.pResults = nullptr;

            vkQueuePresentKHR(PresentationQueues[0], &presentInfo);


        }

        glfwHideWindow(window);
        vkDeviceWaitIdle(device); 
        Helpers::DestroySyncFence(device, F_InFlight);
        Helpers::DestroySyncSemaphore(device, S_RenderFinished);
        Helpers::DestroySyncSemaphore(device, S_ImageAvailable);
        Helpers::DestroyCommandBuffer(device, commandPool, 1, &commandBuffer);
        Helpers::DestroyCommandPool(device, commandPool);
        for (auto& b : frameBuffers) {
            Helpers::DestroyFrameBuffer(device, b);
        }
        Helpers::DestroyPipeline(device, pipeline);
        Helpers::DestroyRenderPass(device, renderPass);
        Helpers::DestroyPipelineLayout(device, pipelineLayout);
        Helpers::DestroyShaderModule(device, FS);
        Helpers::DestroyShaderModule(device, VS);
        for (auto& v : imageViews) {
            Helpers::DestroyImageView(device, v);
        }
        Helpers::DestroySwapChain(device, SwapChain);
        Helpers::DestroyWindowSurface(instance, surface);
        Helpers::DestroyLogicalDevice(device);
        Helpers::DestroyDebugLogger(instance, debugLogger);
        Helpers::DestroyInstance(instance);

        glfwDestroyWindow(window);
        glfwTerminate(); 

    }
    catch(std::exception& e){
        Log::Failure(e.what());
    }
 }
