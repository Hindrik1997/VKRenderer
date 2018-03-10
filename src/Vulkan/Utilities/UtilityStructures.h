//
// Created by Hindrik Stegenga on 7-11-17.
//

#ifndef VKRENDERER_UTILITYSTRUCTURES_H
#define VKRENDERER_UTILITYSTRUCTURES_H

#include "../Platform/VulkanPlatform.h"

struct vk_QueueFamily final {
    uint32_t                queueFamilyIndex        = UINT32_MAX;
    VkQueueFamilyProperties queueFamilyProperties   = {};
};

struct vk_Queue final {
    VkQueue         queue       = VK_NULL_HANDLE;
    vk_QueueFamily  queueFamily = {};
};

struct vk_SwapchainSettings final {

    VkExtent2D                      extent          = {};
    VkSurfaceFormatKHR              surfaceFormat   = {};
    VkPresentModeKHR                presentMode     = {};
    uint32_t                        imageCount      = 0;
    VkSurfaceTransformFlagBitsKHR   preTransform    = {};

};

struct vk_RendermodeSwapchainInfo {

    uint32_t            width               = 0;
    uint32_t            height              = 0;
    VkFormat            format              = VK_FORMAT_UNDEFINED;
    vector<VkImageView> colorImageViews     = {};

};

#endif //VKRENDERER_UTILITYSTRUCTURES_H
