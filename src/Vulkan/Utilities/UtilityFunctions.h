//
// Created by Hindrik Stegenga on 3-11-17.
//

#ifndef VKRENDERER_VULKANUTILITYFUNCTIONS_H
#define VKRENDERER_VULKANUTILITYFUNCTIONS_H

#include "../Platform/VulkanPlatform.h"
#include <iostream>

using std::ostream;

auto operator<<(ostream& o, VkPhysicalDeviceType& t) -> ostream&;

auto operator<<(ostream& o, VkPhysicalDeviceProperties& p) -> ostream&;

auto operator<<(ostream& o, VkQueueFamilyProperties p) -> ostream&;

auto operator<<(ostream& o, VkExtensionProperties p) -> ostream&;

uint32_t deviceTypePriority(VkPhysicalDeviceType deviceType);

VkComponentMapping defaultComponentMapping();

#endif //VKRENDERER_VULKANUTILITYFUNCTIONS_H
