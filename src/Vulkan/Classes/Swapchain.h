//
// Created by hindrik on 25-11-17.
//

#ifndef VKRENDERER_SWAPCHAIN_H
#define VKRENDERER_SWAPCHAIN_H

#include "../Platform/VulkanPlatform.h"

class Swapchain final {
public:
    Swapchain(uint32_t width, uint32_t height, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
    ~Swapchain() = default;

    Swapchain(Swapchain&&)      = default;
    Swapchain(const Swapchain&) = delete;

    Swapchain& operator=(Swapchain&&) noexcept  = default;
    Swapchain& operator=(const Swapchain&)      = delete;
private:
    VKUH<VkSwapchainKHR>    swapchain;
    VkDevice                device;
    VkPhysicalDevice        physicalDevice;
    VkSurfaceKHR            surface;

private:
    bool checkSupport();

};


#endif //VKRENDERER_SWAPCHAIN_H