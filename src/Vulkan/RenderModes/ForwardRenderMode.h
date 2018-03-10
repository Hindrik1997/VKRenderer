//
// Created by hindrik on 12-11-17.
//

#ifndef VKRENDERER_FORWARDRENDERMODE_H
#define VKRENDERER_FORWARDRENDERMODE_H


#include "../Classes/VulkanRenderMode.h"
#include "VulkanRenderModeCreateInfo.h"

class ForwardRenderMode : public VulkanRenderMode {
private:
    vector<VKUH<VkPipelineLayout>> pipelineLayouts;
    vector<VKUH<VkRenderPass>> renderPasses;
    vector<VKUH<VkPipeline>> pipelines;
public:
    explicit ForwardRenderMode(const ForwardRenderModeCreateInfo& createInfo);
    ~ForwardRenderMode() = default;
    ForwardRenderMode(const ForwardRenderMode&) = delete;
    ForwardRenderMode(ForwardRenderMode&&) = default;

    ForwardRenderMode& operator=(const ForwardRenderMode&) = delete;
    ForwardRenderMode& operator=(ForwardRenderMode&&) = default;
public:
    void render() override;

    void windowHasResized(vk_RendermodeSwapchainInfo swapchainInfo) override;
private:
    void createRenderMode(vk_RendermodeSwapchainInfo swapchainInfo);


};


#endif //VKRENDERER_FORWARDRENDERMODE_H
