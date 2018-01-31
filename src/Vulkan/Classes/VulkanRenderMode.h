//
// Created by hindrik on 12-11-17.
//

#ifndef VKRENDERER_VULKANRENDERMODE_H
#define VKRENDERER_VULKANRENDERMODE_H

#include "../Platform/VulkanPlatform.h"
#include "Swapchain.h"
#include "../../Utilities/Nullable.h"

class VulkanRenderMode {
private:
    string                      name;
    VkDevice                    device;
    VkPhysicalDevice            physicalDevice;
    VkSurfaceKHR                surface;

    vector<VKUH<VkRenderPass>>  renderPasses;
    vector<VKUH<VkPipeline>>    pipelines;
    vector<VkCommandPool>       commandPools;

    Nullable<Swapchain>         swapchain;
public:
    VulkanRenderMode(string renderModeName, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    virtual ~VulkanRenderMode() = 0;

    VulkanRenderMode(const VulkanRenderMode&)       = delete;
    VulkanRenderMode(VulkanRenderMode&&) noexcept   = default;

    VulkanRenderMode& operator=(const VulkanRenderMode&)    = delete;
    VulkanRenderMode& operator=(VulkanRenderMode&&)         = default;

public:
    virtual void render() = 0;
    virtual void windowHasResized(uint32_t width, uint32_t height) = 0;
};

inline VulkanRenderMode::~VulkanRenderMode() = default;

#endif //VKRENDERER_VULKANRENDERMODE_H
