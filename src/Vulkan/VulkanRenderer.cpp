//
// Created by Hindrik Stegenga on 29-10-17.
//

#include "VulkanRenderer.h"
#include "../Utilities/ConfigFileReader.h"
#include "RenderModes/ForwardRenderMode.h"
#include "Utilities/Parsable/VertexLayout.h"
#include "../Utilities/ObjLoader.h"


VulkanRenderer::VulkanRenderer(string appName, string engineName,  bool debugEnabled) : debugEnabled(debugEnabled ? VK_TRUE : VK_FALSE)
{

    auto configReader = ConfigFileReader();

    configReader.parseFile("config/vulkan.cfg");

    auto map = configReader.map();

    map.insert(std::make_pair("appName",appName));
    map.insert(std::make_pair("engineName", engineName));

    map.insert(std::make_pair("debug", this->debugEnabled == VK_TRUE ? "true" : "false"));

    uint32_t width, height;
    width = static_cast<uint32_t>(std::stoi(map.at("width")));
    height = static_cast<uint32_t>(std::stoi(map.at("height")));

    renderWindow = RenderWindow(width, height, true);
    renderWindow.setRendererPointer(this);

    vector<const char*> extensions;
	vector<const char*> layers;
	vector<const char*> debugExtensions;
	vector<const char*> debugLayers;
	vector<const char*> dExtensions;
	vector<const char*> ddExtensions;

#ifdef USE_INSTANCE_EXT
    for (auto instanceExtension : instanceExtensions) {
        extensions.push_back(instanceExtension);
    }
#endif
#ifdef USE_INSTANCE_LAYERS
    for (auto instanceLayer : instanceLayers) {
        layers.push_back(instanceLayer);
    }
#endif
#ifdef USE_DEBUG_INSTANCE_EXT
    for (auto instanceExtension : debugInstanceExtensions) {
        debugExtensions.push_back(instanceExtension);
    }
#endif
#ifdef USE_DEBUG_INSTANCE_LAYERS
    for (auto instanceLayer : debugInstanceLayers) {
        debugLayers.push_back(instanceLayer);
    }
#endif
#ifdef USE_DEVICE_EXT
    for (auto extension : deviceExtensions) {
        dExtensions.push_back(extension);
    }
#endif
#ifdef USE_DEBUG_DEVICE_EXT
    for (auto extension : debugDeviceExtensions) {
        ddExtensions.push_back(extension);
    }
#endif

    auto processedExtensions = renderWindow.processExtensions(extensions);

    VulkanInstanceCreateInfo supportDescription { layers, processedExtensions, debugLayers, debugExtensions };

    instance = Instance(map, supportDescription);

    if (this->debugEnabled == VK_TRUE) {
        setupDebugCallback();
    }

    VkSurfaceKHR surface = renderWindow.getWindowSurface(instance.getHandle());

    PresentDeviceCreateInfo deviceSupportDescription { dExtensions, ddExtensions, requiredDeviceFeatures, surface };

    device = PresentDevice(instance.getHandle(), map, deviceSupportDescription);

    SwapchainCreateInfo swapchainCreateInfo = {};

    swapchainCreateInfo.deviceInfo  = device.getPresentDeviceInfo();
    swapchainCreateInfo.surface     = device.getSurface();
    swapchainCreateInfo.width       = width;
    swapchainCreateInfo.height      = height;

    if (map.find("preferredFramesInFlight") != map.end()) {
        swapchainCreateInfo.preferredFramesInFlight = (uint32_t)std::stoi(map.at("preferredFramesInFlight"));
    } else {
        swapchainCreateInfo.preferredFramesInFlight = 2;
    }

    swapchain = Swapchain(swapchainCreateInfo);
    
    const string renderMode = configReader.map().at("renderMode");

    if (renderMode == string("Forward")) {

        ForwardRenderModeCreateInfo createInfo = {};

        createInfo.deviceInfo       = device.getPresentDeviceInfo();
        createInfo.surface          = surface;
        createInfo.swapchainInfo    = swapchain.getRendermodeSwapchainInfo();

        this->renderMode = std::make_unique<ForwardRenderMode>(createInfo);
    }
    else
    {
        Logger::error("Could not find appropriate renderMode!");
    }
}

bool VulkanRenderer::processEvents(std::chrono::nanoseconds deltaTime)
{
    renderWindow.setRendererPointer(this);

    accumulatedTime += deltaTime;
    accumulatedFrames += 1;

    if (accumulatedTime > std::chrono::milliseconds(500)) {

        nanoseconds averagePerFrame = accumulatedTime / accumulatedFrames;

        nanoseconds second = std::chrono::duration_cast<nanoseconds>(std::chrono::seconds(1));

        if (averagePerFrame.count() > 0) {
            auto fps = second.count() / averagePerFrame.count();

            renderWindow.setWindowTitle("VKRenderer " + std::to_string(fps) + " FPS");

            accumulatedFrames = 0;
            accumulatedTime = nanoseconds(0);
        }
    }

    return renderWindow.pollWindowEvents();
}

VkBool32 VulkanRenderer::debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj,
                                       size_t location, int32_t code, const char *layerPrefix, const char *msg,
                                       void *userData)
{
    Logger::warn("Validation layer: " + string(msg));
    return VK_FALSE;
}

void VulkanRenderer::resizeWindow(uint32_t width, uint32_t height) {

    if(width == 0 && height == 0)
        return;

    Logger::log("Window will be resized: " + std::to_string(width) + " - " + std::to_string(height));

    vk_RendermodeSwapchainInfo swapchainInfo = swapchain.recreateSwapchain(width, height, swapchain.preferredFramesInFlight());

    renderMode->windowHasResized(swapchainInfo);
}

void VulkanRenderer::resizeWindow(bool mustResize) {
    if(mustResize)
    {
        uint32_t width, height;
        renderWindow.getCurrentSize(width, height);
        resizeWindow(width, height);
    }
}

void VulkanRenderer::setupDebugCallback()
{
    VkDebugReportCallbackCreateInfoEXT createInfo = {};
    createInfo.sType        = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.pNext        = nullptr;
    createInfo.flags        = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback  = debugCallback;
    createInfo.pUserData    = nullptr;

    VkResult result = createDebugReportCallbackEXT(instance.getHandle(), &createInfo, nullptr, &debugCallbackHandle);
    if (result != VK_SUCCESS) {
        Logger::warn("Debug callback is not enabled! Reason: " + mapVkResult(result));
        debugEnabled = VK_FALSE;
    }
    else {
        Logger::success("Debug callback enabled!");
    }
}

VulkanRenderer::~VulkanRenderer()
{
    if (instance.getHandle() != VK_NULL_HANDLE) {
        Logger::log("Shutting down the Vulkan renderer.");
        vkDeviceWaitIdle(getDevice());
    }

    if (debugEnabled == VK_TRUE) {
        destroyDebugReportCallbackEXT(instance.getHandle(), debugCallbackHandle, nullptr);
    }
}

void VulkanRenderer::render()
{
    vk_PresentImageInfo presentImageInfo = swapchain.acquireNextImage();

    resizeWindow(presentImageInfo.mustRecreateSwapchain);

    if(presentImageInfo.imageIndex == std::numeric_limits<uint32_t>::max())
        return;

    renderMode->render(presentImageInfo);

    bool mustResize = false;
    swapchain.presentImage(presentImageInfo.imageIndex, mustResize);

    resizeWindow(mustResize);
}

VkDevice VulkanRenderer::getDevice() {
    return device.getPresentDeviceInfo().logical;
}