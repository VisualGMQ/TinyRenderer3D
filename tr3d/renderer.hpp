#pragma once
#include <utility>
#include <iostream>
#include <vector>
#include <optional>
#include <unordered_set>
#include <unordered_map>
#include <limits>
#include <algorithm>

#include "vulkan/vulkan.hpp"
#include "SDL.h"
#include "SDL_vulkan.h"

#define LOG(fmt, ...) printf("[%d][%s]: " fmt "\n", __LINE__, __FUNCTION__, ##__VA_ARGS__)

namespace tr3d {

class Renderer final {
public:
    static void Init(SDL_Window*);
    static void Quit();

    static vk::ShaderModule CreateShaderModule(const char* name, const std::vector<char>& code);
    static void InitPipeline(vk::ShaderModule vertShader, vk::ShaderModule fragShader);
    static void DrawFrame();
    static void WaitIdle();

private:
    static vk::Instance instance_;
    static vk::SurfaceKHR surface_;
    static vk::PhysicalDevice phyDevice_;
    static vk::Device device_;
    static vk::Queue graphicQueue_;
    static vk::Queue presentQueue_;
    static vk::SwapchainKHR swapchain_;
    static vk::PipelineLayout layout_;
    static vk::Pipeline pipeline_;
    static vk::RenderPass renderPass_;
    static std::vector<vk::Image> images_;
    static std::vector<vk::ImageView> imageViews_;
    static std::vector<vk::Framebuffer> frambuffers_;
    static vk::CommandBuffer cmdBuf_;
    static vk::CommandPool cmdPool_;
    static vk::Semaphore imageAvaliableSemaphore_;
    static vk::Semaphore renderFinishedSemaphore_;
    static vk::Fence inFlightFence_;

    static SDL_Window* window_;

    static std::unordered_map<const char*, vk::ShaderModule> shaderModules_;


    struct QueueFamily {
        std::optional<std::uint32_t> presentQueueIdx;
        std::optional<std::uint32_t> graphicQueueIdx;
    };

    struct SwapchainSupportDetails {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };

    struct SwapchainRequireDetails {
        vk::Extent2D extent;
        vk::SurfaceFormatKHR format;
        vk::PresentModeKHR presentMode;
    };
    static SwapchainRequireDetails swapchainRequiredDetails_;

    static vk::Instance createInstance();
    static vk::SurfaceKHR createSurface();
    static vk::PhysicalDevice pickupPhysicalDevice();
    static QueueFamily queryQueueFamily();
    static vk::Device createDevice(QueueFamily queueFamily);
    static SwapchainSupportDetails querySwapchainSupport();
    static vk::SurfaceFormatKHR chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats);
    static vk::PresentModeKHR choosePresentMode(const std::vector<vk::PresentModeKHR>&);
    static vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR&);
    static std::pair<vk::Queue, vk::Queue> getQueues(QueueFamily queueFamily);
    static SwapchainRequireDetails querySwapchainRequiredDetails(const SwapchainSupportDetails&);
    static vk::SwapchainKHR createSwapchain(const SwapchainSupportDetails&, const SwapchainRequireDetails&, const QueueFamily&);
    static std::vector<vk::ImageView> createImageViews(vk::Format);
    static std::vector<vk::Image> getImages();
    static vk::RenderPass createRenderPass();
    static std::vector<vk::Framebuffer> createFramebuffers();
    static vk::CommandPool createCmdPool(const QueueFamily& queueFamily);
    static vk::CommandBuffer createCmdBuffer();
    static void recordCmdBuffer(uint32_t imageIdx);
    static std::tuple<vk::Semaphore, vk::Semaphore, vk::Fence> createSemaphoreAndFence();
};

}
