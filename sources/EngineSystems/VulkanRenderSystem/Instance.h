//
// Created by Hindrik Stegenga on 2-11-17.
//

#ifndef VKRENDERER_VULKANINSTANCE_H
#define VKRENDERER_VULKANINSTANCE_H

#include "Platform/VulkanPlatform.h"

class Instance final {
private:
    VKUH<VkInstance> instance = {};
public:
    Instance() = default;
    Instance(vk_GeneralSettings settings, const VulkanInstanceCreateInfo& createInfo);
    ~Instance() = default;

    Instance(const Instance&)       = delete;
    Instance(Instance&&) noexcept   = default;

    Instance& operator=(const Instance&)        = delete;
    Instance& operator=(Instance&&) noexcept    = default;
private:
    static void checkLayersAndExtensionsSupport(const vector<const char*>& validationLayers, const vector<const char*>& extensions);
    static pair<bool, string> checkValidationLayerSupport(const vector<const char*>& layers);
    static pair<bool, string> checkExtensionSupport(const vector<const char*>& validationLayers, const vector<const char*>& extensions);
public:
    VkInstance getHandle() const;
};


#endif //VKRENDERER_VULKANINSTANCE_H
