//
// Created by hindrik on 12-11-17.
//

#include "ForwardRenderMode.h"

void ForwardRenderMode::render() {

}

ForwardRenderMode::ForwardRenderMode(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
        : VulkanRenderMode("Forward", device, physicalDevice, surface)
{

}
