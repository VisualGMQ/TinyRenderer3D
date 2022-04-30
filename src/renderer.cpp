#include "tr3d/renderer.hpp"

namespace tr3d {

vk::Instance Renderer::instance_ = nullptr;
vk::SurfaceKHR Renderer::surface_ = nullptr;
vk::PhysicalDevice Renderer::phyDevice_ = nullptr;
vk::Device Renderer::device_ = nullptr;
vk::Queue Renderer::presentQueue_ = nullptr;
vk::Queue Renderer::graphicQueue_ = nullptr;
vk::PipelineLayout Renderer::layout_;
vk::Pipeline Renderer::pipeline_ = nullptr;
vk::RenderPass Renderer::renderPass_ = nullptr;
vk::SwapchainKHR Renderer::swapchain_ = nullptr;
vk::CommandBuffer Renderer::cmdBuf_ = nullptr;
vk::CommandPool Renderer::cmdPool_ = nullptr;
std::unordered_map<const char*, vk::ShaderModule> Renderer::shaderModules_;
std::vector<vk::Image> Renderer::images_;
std::vector<vk::ImageView> Renderer::imageViews_;
std::vector<vk::Framebuffer> Renderer::frambuffers_;
vk::Semaphore Renderer::imageAvaliableSemaphore_ = nullptr;
vk::Semaphore Renderer::renderFinishedSemaphore_ = nullptr;
vk::Fence Renderer::inFlightFence_ = nullptr;

vk::ShaderModule Renderer::vertexShader_ = nullptr;
vk::ShaderModule Renderer::fragShader_ = nullptr;

SDL_Window* Renderer::window_ = nullptr;

std::unordered_map<const char*, vk::ShaderModule> shaderModules_;
Renderer::SwapchainRequireDetails Renderer::swapchainRequiredDetails_;
Renderer::SwapchainSupportDetails Renderer::swapchainSupportDetails_;
Renderer::QueueFamily Renderer::queueFamily_;

std::vector<const char*> extensionNames = {"VK_KHR_get_physical_device_properties2"};
std::vector<const char*> layerNames = { "VK_LAYER_KHRONOS_validation" };

void Renderer::Init(SDL_Window* window) {
    window_ = window;

    unsigned int count;
    SDL_Vulkan_GetInstanceExtensions(window_, &count, nullptr);
    size_t size = extensionNames.size();
    extensionNames.resize(count + extensionNames.size());
    SDL_Vulkan_GetInstanceExtensions(window_, &count, extensionNames.data() + size);

    if (!(instance_ = createInstance())) {
        LOG("create instance failed");
    } else {
        LOG("create instance OK!");
    }

    if (!(surface_ = createSurface())) {
        LOG("create surface failed");
    } else {
        LOG("create surface OK!");
    }

    if ((phyDevice_ = pickupPhysicalDevice())) {
        LOG("pickup device %s OK", phyDevice_.getProperties().deviceName.data());
    } else {
        LOG("pickup physical device failed");
    }

    queueFamily_ = queryQueueFamily();
    LOG("graphic family queue index = %d", queueFamily_.graphicQueueIdx.value());
    LOG("present family queue index = %d", queueFamily_.presentQueueIdx.value());

    if ((device_ = createDevice(queueFamily_))) {
        LOG("create logical device OK");
    } else {
        LOG("create logical device failed");
    }

    auto queues = getQueues(queueFamily_);
    graphicQueue_ = queues.first;
    presentQueue_ = queues.second;
    swapchainSupportDetails_ = querySwapchainSupport();
    swapchainRequiredDetails_ = querySwapchainRequiredDetails(swapchainSupportDetails_);
    if ((swapchain_ = createSwapchain(swapchainSupportDetails_, swapchainRequiredDetails_, queueFamily_))) {
        LOG("swapchain create OK");
    } else {
        LOG("swapchain create failed");
    }

    images_ = getImages();
    LOG("swapchain has %lu images", images_.size());

    imageViews_ = createImageViews(swapchainRequiredDetails_.format.format);

    if ((renderPass_ = createRenderPass())) {
        LOG("render pass create OK");
    } else {
        LOG("render pass create failed");
    }

    frambuffers_ = createFramebuffers();

    if ((cmdPool_ = createCmdPool(queueFamily_))) {
        LOG("graphic command pool create OK");
    } else {
        LOG("graphic command pool create failed");
    }

    if ((cmdBuf_ = createCmdBuffer())) {
        LOG("create command buffer OK");
    } else {
        LOG("create command buffer failed");
    }

    auto tup = createSemaphoreAndFence();
    imageAvaliableSemaphore_ = std::get<0>(tup);
    renderFinishedSemaphore_ = std::get<1>(tup);
    inFlightFence_ = std::get<2>(tup);
}

vk::Instance Renderer::createInstance() {
    vk::InstanceCreateInfo info;
    info.setPEnabledLayerNames(layerNames);
    info.setPEnabledExtensionNames(extensionNames);

    std::cout << "enabled extensions:" << std::endl;
    for (auto& ext : extensionNames) {
        std::cout << "\t" << ext << std::endl;
    }

    std::cout << "enabled layers:" << std::endl;
    for (auto& layer : layerNames) {
        std::cout << "\t" << layer << std::endl;
    }

    return vk::createInstance(info);
}

vk::SurfaceKHR Renderer::createSurface() {
    VkSurfaceKHR surface;
    SDL_Vulkan_CreateSurface(window_, instance_, &surface);
    return surface;
}

vk::PhysicalDevice Renderer::pickupPhysicalDevice() {
    return instance_.enumeratePhysicalDevices()[0];
}

Renderer::QueueFamily Renderer::queryQueueFamily() {
    auto properties = phyDevice_.getQueueFamilyProperties();
    QueueFamily info;
    for (size_t i = 0; i < properties.size(); i++) {
        auto& property = properties[i];
        if (property.queueFlags | vk::QueueFlagBits::eGraphics) {
            info.graphicQueueIdx = i;
            if (phyDevice_.getSurfaceSupportKHR(i, surface_)) {
                info.presentQueueIdx = i;
            }
            if (info.presentQueueIdx && info.graphicQueueIdx) {
                break;
            }
        }
    }
    return info;
}

vk::Device Renderer::createDevice(Renderer::QueueFamily queueFamily) {
    std::unordered_set<std::uint32_t> families = {queueFamily.graphicQueueIdx.value(), queueFamily.presentQueueIdx.value()};
    std::vector<vk::DeviceQueueCreateInfo> createInfos;
    
    for (auto& index : families) {
        vk::DeviceQueueCreateInfo info;
        std::array<float, 1> priorities = {1.0};
        info.setQueueFamilyIndex(index);
        info.setQueueCount(1);
        info.setQueuePriorities(priorities);

        createInfos.push_back(info);
    }

    std::array extensionNames = {"VK_KHR_portability_subset", VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    vk::DeviceCreateInfo info;
    info.setQueueCreateInfos(createInfos);
    info.setPEnabledExtensionNames(extensionNames);

    return phyDevice_.createDevice(info);
}

std::pair<vk::Queue, vk::Queue> Renderer::getQueues(QueueFamily queueFamily) {
    return {device_.getQueue(queueFamily.graphicQueueIdx.value(), 0),
            device_.getQueue(queueFamily.presentQueueIdx.value(), 0)};
}

Renderer::SwapchainSupportDetails Renderer::querySwapchainSupport() {
    SwapchainSupportDetails details;
    details.formats = phyDevice_.getSurfaceFormatsKHR(surface_);
    details.presentModes = phyDevice_.getSurfacePresentModesKHR(surface_);
    details.capabilities = phyDevice_.getSurfaceCapabilitiesKHR(surface_);
    return details;
}

vk::SurfaceFormatKHR Renderer::chooseSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& formats) {
    for (auto& format : formats) {
        if ((format.format == vk::Format::eR8G8B8A8Srgb ||
            format.format == vk::Format::eB8G8R8A8Srgb) &&
            format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
            return format;
    }
    return formats[0];
}

vk::PresentModeKHR Renderer::choosePresentMode(const std::vector<vk::PresentModeKHR>& modes) {
    for (auto& mode : modes) {
        if (mode == vk::PresentModeKHR::eMailbox) {
            return mode;
        }
    }
    return vk::PresentModeKHR::eFifo;
}

vk::Extent2D Renderer::chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities) {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        return capabilities.currentExtent;
    } else {
        int w, h;
        SDL_GetWindowSize(window_, &w, &h);
        vk::Extent2D extent;
        extent.width = std::clamp(static_cast<uint32_t>(w),
                                  capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extent.height = std::clamp(static_cast<uint32_t>(h),
                                   capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
        return extent;
    }
}

Renderer::SwapchainRequireDetails Renderer::querySwapchainRequiredDetails(const SwapchainSupportDetails& swapchainDetails) {
    SwapchainRequireDetails details;
    details.extent = chooseSwapExtent(swapchainDetails.capabilities);
    details.format = chooseSurfaceFormat(swapchainDetails.formats);
    details.presentMode = choosePresentMode(swapchainDetails.presentModes);
    return details;
}

vk::SwapchainKHR Renderer::createSwapchain(const SwapchainSupportDetails& supportDetails, const SwapchainRequireDetails& details, const QueueFamily& queueFamily) {
    uint32_t imageCount = supportDetails.capabilities.minImageCount + 1;
    if (supportDetails.capabilities.maxImageCount > 0 &&
        imageCount > supportDetails.capabilities.maxImageCount) {
        imageCount = supportDetails.capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR info;
    info.setSurface(surface_);
    info.setMinImageCount(imageCount);
    info.setImageArrayLayers(1);
    info.setImageExtent(details.extent);
    info.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);
    info.setPreTransform(supportDetails.capabilities.currentTransform);
    info.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);
    info.setPresentMode(details.presentMode);
    info.setImageFormat(details.format.format);
    info.setImageColorSpace(details.format.colorSpace);
    info.setClipped(true);

    if (queueFamily.graphicQueueIdx.value() != queueFamily.presentQueueIdx.value()) {
        info.setImageSharingMode(vk::SharingMode::eConcurrent);
        info.setQueueFamilyIndexCount(2);
        info.setQueueFamilyIndices(std::array<uint32_t, 2>({queueFamily.presentQueueIdx.value(),
                                                            queueFamily.graphicQueueIdx.value()}));
    } else {
        info.setImageSharingMode(vk::SharingMode::eExclusive);
        info.setQueueFamilyIndexCount(0);
        info.setQueueFamilyIndices(nullptr);
    }

    return device_.createSwapchainKHR(info);
}

void Renderer::InitPipeline(vk::ShaderModule vertShader, vk::ShaderModule fragShader) {
    if (!vertShader || !fragShader) {
        LOG("shader is nullptr");
    }

    vertexShader_ = vertShader;
    fragShader_ = fragShader;

    // vertex input
    vk::PipelineVertexInputStateCreateInfo vertexInputStateInfo;

    // vertex assmebly
    vk::PipelineInputAssemblyStateCreateInfo inputAsmInfo;
    inputAsmInfo.setTopology(vk::PrimitiveTopology::eTriangleList);
    inputAsmInfo.setPrimitiveRestartEnable(false);

    // viewport transform and clip
    vk::PipelineViewportStateCreateInfo viewportInfo;
    int w, h;
    SDL_GetWindowSize(window_, &w, &h);
    vk::Viewport viewport(0, 0, w, h, 0, 1);
    vk::Rect2D scissor({0, 0}, swapchainRequiredDetails_.extent);
    viewportInfo.setViewports(viewport);
    viewportInfo.setScissors(scissor);

    // rasterization
    vk::PipelineRasterizationStateCreateInfo rastInfo;
    rastInfo.setLineWidth(1);
    rastInfo.setFrontFace(vk::FrontFace::eCounterClockwise);
    rastInfo.setCullMode(vk::CullModeFlagBits::eFront);
    rastInfo.setPolygonMode(vk::PolygonMode::eFill);
    rastInfo.setDepthClampEnable(false);
    rastInfo.setDepthBiasEnable(false);
    rastInfo.setRasterizerDiscardEnable(false);

    // multisampling
    vk::PipelineMultisampleStateCreateInfo multisampInfo;
    multisampInfo.setSampleShadingEnable(false);
    multisampInfo.setMinSampleShading(1);
    multisampInfo.setRasterizationSamples(vk::SampleCountFlagBits::e1);

    // depth test, stencil test, alpha test

    // blend  NOTE: understand this
    vk::PipelineColorBlendAttachmentState blendState;
    blendState.setBlendEnable(false);
    blendState.setColorWriteMask(vk::ColorComponentFlagBits::eA | vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB);
    vk::PipelineColorBlendStateCreateInfo blendCreateInfo;
    blendCreateInfo.setAttachments(blendState);
    blendCreateInfo.setLogicOpEnable(false);

    // pipeline layout(uniform values and constant values)
    vk::PipelineLayoutCreateInfo layoutInfo;
    layout_ = device_.createPipelineLayout(layoutInfo);

    // init shader stages
    std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStageCreateInfo;
    shaderStageCreateInfo[0].setModule(vertShader);
    shaderStageCreateInfo[0].setPName("main");
    shaderStageCreateInfo[0].setStage(vk::ShaderStageFlagBits::eVertex);
    shaderStageCreateInfo[1].setModule(fragShader);
    shaderStageCreateInfo[1].setPName("main");
    shaderStageCreateInfo[1].setStage(vk::ShaderStageFlagBits::eFragment);

    vk::GraphicsPipelineCreateInfo info;
    info.setStages(shaderStageCreateInfo);

    info.setPVertexInputState(&vertexInputStateInfo);
    info.setPInputAssemblyState(&inputAsmInfo);
    info.setPViewportState(&viewportInfo);
    info.setPRasterizationState(&rastInfo);
    info.setPMultisampleState(&multisampInfo);
    info.setPDepthStencilState(nullptr);
    info.setPColorBlendState(&blendCreateInfo);
    info.setLayout(layout_);
    info.setRenderPass(renderPass_);
    info.setSubpass(0);
    info.setBasePipelineIndex(-1);

    auto result = device_.createGraphicsPipeline(nullptr, info);
    if (result.result != vk::Result::eSuccess) {
        LOG("create graphics pipeline failed");
        pipeline_ = nullptr;
    } else {
        pipeline_ = result.value;
    }
}

std::vector<vk::Image> Renderer::getImages() {
    return device_.getSwapchainImagesKHR(swapchain_);
}

std::vector<vk::ImageView> Renderer::createImageViews(vk::Format format) {
    std::vector<vk::ImageView> imageViews(images_.size()); 
    for (size_t i = 0; i < images_.size(); i++) {
        vk::ImageViewCreateInfo info;
        info.setImage(images_[i]);
        info.setViewType(vk::ImageViewType::e2D);
        info.setFormat(format);
        vk::ComponentMapping mapping;
        mapping.setA(vk::ComponentSwizzle::eIdentity);
        mapping.setR(vk::ComponentSwizzle::eIdentity);
        mapping.setG(vk::ComponentSwizzle::eIdentity);
        mapping.setB(vk::ComponentSwizzle::eIdentity);
        info.setComponents(mapping);
        vk::ImageSubresourceRange range;
        range.setAspectMask(vk::ImageAspectFlagBits::eColor);
        range.setBaseMipLevel(0);
        range.setBaseArrayLayer(0);
        range.setLayerCount(1);
        range.setLevelCount(1);
        info.setSubresourceRange(range);

        imageViews[i] = device_.createImageView(info);
    }
    return imageViews;
}

vk::RenderPass Renderer::createRenderPass() {
    vk::AttachmentDescription colorAttachment;
    colorAttachment.setFormat(swapchainRequiredDetails_.format.format);
    colorAttachment.setLoadOp(vk::AttachmentLoadOp::eClear);
    colorAttachment.setStoreOp(vk::AttachmentStoreOp::eStore);
    colorAttachment.setSamples(vk::SampleCountFlagBits::e1);
    colorAttachment.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
    colorAttachment.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
    colorAttachment.setInitialLayout(vk::ImageLayout::eUndefined);
    colorAttachment.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);

    vk::AttachmentReference colorAttachmentRef;
    colorAttachmentRef.setAttachment(0);
    colorAttachmentRef.setLayout(vk::ImageLayout::eColorAttachmentOptimal);

    vk::SubpassDescription subpassDescription;
    subpassDescription.setColorAttachments(std::array<vk::AttachmentReference, 1>({colorAttachmentRef}));

    vk::SubpassDependency dependency;
    dependency.setSrcSubpass(VK_SUBPASS_EXTERNAL);
    dependency.setDstSubpass(0);
    dependency.setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
    dependency.setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput);
    dependency.setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite);

    vk::RenderPassCreateInfo info;
    info.setAttachments(std::array<vk::AttachmentDescription, 1>({colorAttachment}));
    info.setSubpasses(std::array<vk::SubpassDescription, 1>({subpassDescription}));
    info.setDependencies(std::array<vk::SubpassDependency, 1>{dependency});

    return device_.createRenderPass(info);
}

std::vector<vk::Framebuffer> Renderer::createFramebuffers() {
    std::vector<vk::Framebuffer> buffers(imageViews_.size());

    for (size_t i = 0; i < imageViews_.size(); i++) {
        vk::FramebufferCreateInfo info;
        info.setAttachments(std::array<vk::ImageView, 1>({imageViews_[i]}));
        info.setRenderPass(renderPass_);
        info.setLayers(1);
        info.setWidth(swapchainRequiredDetails_.extent.width);
        info.setHeight(swapchainRequiredDetails_.extent.height);

        buffers[i] = device_.createFramebuffer(info);
        if (!buffers[i]) {
            LOG("create framebuffer failed");
        }
    }
    return buffers;
}

vk::CommandPool Renderer::createCmdPool(const QueueFamily& queueFamily) {
    vk::CommandPoolCreateInfo info;
    info.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);
    info.setQueueFamilyIndex(queueFamily.graphicQueueIdx.value());

    return device_.createCommandPool(info);
}

vk::CommandBuffer Renderer::createCmdBuffer() {
    vk::CommandBufferAllocateInfo info;
    info.setCommandBufferCount(1);
    info.setCommandPool(cmdPool_);
    info.setLevel(vk::CommandBufferLevel::ePrimary);

    return device_.allocateCommandBuffers(info)[0];
}

void Renderer::recordCmdBuffer(uint32_t imageIdx) {
    vk::CommandBufferBeginInfo beginInfo;

    // begin record command
    cmdBuf_.begin(beginInfo); {
        vk::RenderPassBeginInfo renderPassBeginInfo;
        renderPassBeginInfo.setRenderPass(renderPass_);
        renderPassBeginInfo.setRenderArea(vk::Rect2D({0, 0}, swapchainRequiredDetails_.extent));

        vk::ClearColorValue value(std::array<float, 4>{0.1, 0.1, 0.1, 1});
        vk::ClearValue clearValue(value);

        renderPassBeginInfo.setClearValues(std::array<vk::ClearValue, 1>{value});
        renderPassBeginInfo.setFramebuffer(frambuffers_[imageIdx]);
        cmdBuf_.beginRenderPass(&renderPassBeginInfo, vk::SubpassContents::eInline); {
            cmdBuf_.bindPipeline(vk::PipelineBindPoint::eGraphics, pipeline_);
            cmdBuf_.draw(3, 1, 0, 0);
        } cmdBuf_.endRenderPass();
    } cmdBuf_.end();
}

std::tuple<vk::Semaphore, vk::Semaphore, vk::Fence> Renderer::createSemaphoreAndFence() {
    vk::SemaphoreCreateInfo semInfo;
    auto sem1 = device_.createSemaphore(semInfo);
    auto sem2 = device_.createSemaphore(semInfo);

    vk::FenceCreateInfo fenceInfo;
    fenceInfo.setFlags(::vk::FenceCreateFlagBits::eSignaled);
    auto fence = device_.createFence(fenceInfo);

    return {sem1, sem2, fence};
}

void Renderer::Quit() {
    device_.destroySemaphore(imageAvaliableSemaphore_);
    device_.destroySemaphore(renderFinishedSemaphore_);
    device_.destroyFence(inFlightFence_);

    cleanupSwapchain();

    for (auto& shaderModule : shaderModules_) {
        device_.destroyShaderModule(shaderModule.second);
    }

    device_.freeCommandBuffers(cmdPool_, 1, &cmdBuf_);
    device_.destroyCommandPool(cmdPool_);
    
    device_.destroy();
    instance_.destroySurfaceKHR(surface_);
    instance_.destroy();
}

vk::ShaderModule Renderer::CreateShaderModule(const char* name, const std::vector<char>& code) {
    if (shaderModules_.find(name) != shaderModules_.end()) {
        LOG("shader module %s already exists", name);
        return nullptr;
    }

    vk::ShaderModuleCreateInfo info;
    info.pCode = (const uint32_t*)code.data();
    info.codeSize = code.size();

    auto shaderModule = device_.createShaderModule(info);
        shaderModules_[name] = shaderModule;
    return shaderModule;
}

void Renderer::DrawFrame() {
    std::array<vk::Fence, 1> fences{inFlightFence_};
    if (device_.waitForFences(fences, true, std::numeric_limits<uint64_t>::max()) != vk::Result::eSuccess) {
        LOG("wait fence failed");
    }
    device_.resetFences(fences);

    uint32_t imageIndex;
    auto result = device_.acquireNextImageKHR(swapchain_, std::numeric_limits<uint64_t>::max(), imageAvaliableSemaphore_);
    if (result.result == vk::Result::eSuccess) {
        imageIndex = result.value;
    } else {
        LOG("acquireNextImage failed");
    }

    cmdBuf_.reset();
    recordCmdBuffer(imageIndex);

    vk::SubmitInfo submitInfo;
    submitInfo.setWaitSemaphores(std::array<vk::Semaphore, 1>{imageAvaliableSemaphore_});
    vk::PipelineStageFlags waitStageFlags[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    submitInfo.setPWaitDstStageMask(waitStageFlags);
    submitInfo.setSignalSemaphores(std::array<vk::Semaphore, 1>{renderFinishedSemaphore_});
    submitInfo.setCommandBuffers(std::array<vk::CommandBuffer, 1>{cmdBuf_});

    graphicQueue_.submit(submitInfo, inFlightFence_);

    vk::PresentInfoKHR presentInfo;    
    presentInfo.setSwapchains(std::array<vk::SwapchainKHR, 1>{swapchain_});
    presentInfo.setWaitSemaphores(std::array<vk::Semaphore, 1>{renderFinishedSemaphore_});
    presentInfo.setImageIndices(std::array<uint32_t, 1>{imageIndex});
    
    if (presentQueue_.presentKHR(presentInfo) != vk::Result::eSuccess) {
        LOG("present failed");
    }
}

void Renderer::cleanupSwapchain() {
    for (auto& buffer : frambuffers_) {
        device_.destroyFramebuffer(buffer);
    }
    device_.destroyPipelineLayout(layout_);
    device_.destroyPipeline(pipeline_);
    device_.destroyRenderPass(renderPass_);
    for (auto& imageView : imageViews_) {
        device_.destroyImageView(imageView);
    }
    device_.destroySwapchainKHR(swapchain_);
}


void Renderer::WaitIdle() {
    device_.waitIdle();
}

void Renderer::OnResize(int w, int h) {
    device_.waitIdle();
    swapchainRequiredDetails_.extent.width = w;
    swapchainRequiredDetails_.extent.height = h;

    cleanupSwapchain();

    swapchain_ = createSwapchain(swapchainSupportDetails_, swapchainRequiredDetails_, queueFamily_);
    images_ = getImages();
    imageViews_ = createImageViews(swapchainRequiredDetails_.format.format);
    renderPass_ = createRenderPass();
    frambuffers_ = createFramebuffers();
    InitPipeline(vertexShader_, fragShader_);
}

}
