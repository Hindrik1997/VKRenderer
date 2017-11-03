//
// Created by Hindrik Stegenga on 2-11-17.
//

#include <cstring>
#include "VulkanInstance.h"

VulkanInstance::VulkanInstance(const map<string, string>& params, const VulkanInstanceSupportDescription& instanceSupportDescription){

    string appName      = params.at("appName");
    string engineName   = params.at("engineName");
    bool debug          = params.at("debug") == "true";

    vector<const char*> usedValidationLayers;
    vector<const char*> usedExtensions;

    if (debug)
    {
        auto dLayers(instanceSupportDescription.validationLayers);
        auto dExtensions(instanceSupportDescription.extensions);

        dLayers.insert(std::end(dLayers), std::begin(instanceSupportDescription.debugValidationLayers), std::end(instanceSupportDescription.debugValidationLayers));
        dExtensions.insert(std::end(dExtensions),std::begin(instanceSupportDescription.debugExtensions), std::end(instanceSupportDescription.debugExtensions));

        checkLayersAndExtensionsSupport(dLayers, dExtensions);

        usedValidationLayers.swap(dLayers);
        usedExtensions.swap(dExtensions);
    } else
    {
        checkLayersAndExtensionsSupport(instanceSupportDescription.validationLayers, instanceSupportDescription.extensions);
        usedValidationLayers = instanceSupportDescription.validationLayers;
        usedExtensions = instanceSupportDescription.extensions;
    }

    VkApplicationInfo applicationInfo   = {};

    applicationInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    applicationInfo.pNext               = nullptr;
    applicationInfo.pApplicationName    = appName.c_str();
    applicationInfo.pEngineName         = appName.c_str();
    applicationInfo.engineVersion       = VK_MAKE_VERSION(1,0,0);
    applicationInfo.applicationVersion  = VK_MAKE_VERSION(1,0,0);
    applicationInfo.apiVersion          = VK_API_VERSION_1_0;

    VkInstanceCreateInfo instanceCreateInfo     = {};

    instanceCreateInfo.sType                    = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceCreateInfo.pNext                    = nullptr;
    instanceCreateInfo.pApplicationInfo         = &applicationInfo;
    instanceCreateInfo.enabledLayerCount        = static_cast<uint32_t>(usedValidationLayers.size());
    instanceCreateInfo.ppEnabledLayerNames      = usedValidationLayers.data();
    instanceCreateInfo.enabledExtensionCount    = static_cast<uint32_t>(usedExtensions.size());
    instanceCreateInfo.ppEnabledExtensionNames  = usedExtensions.data();

    auto result = vkCreateInstance(&instanceCreateInfo, nullptr, instance.reset());
    handleResult(result, "Instance creation failed!");

    for(auto& item : usedValidationLayers)
    {
        auto itemstr = string(item);
        auto pre = string("Enabled validation layer: ");
        Logger::log(pre + itemstr);
    }

    for(auto& item : usedExtensions)
    {
        auto itemstr = string(item);
        auto pre = string("Enabled instance extension: ");
        Logger::log(pre + itemstr);
    }

    Logger::succes("Vulkan instance creation succeeded!");
}

pair<bool, string> VulkanInstance::checkValidationLayerSupport(const vector<const char *> &layers)
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    vector<VkLayerProperties> available(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, available.data());

    for (const char* layerName : layers)
    {
        bool isFound = false;
        for (const auto& layerProps : available)
        {
            if (strcmp(layerName, layerProps.layerName) == 0)
            {
                isFound = true;
                break;
            }
        }
        if (!isFound)
        {
            return make_pair(false, string(layerName));
        }
    }
    return make_pair(true, "SUCCESS");
}
pair<bool, string> VulkanInstance::checkExtensionSupport(const vector<const char*>& validationLayers, const vector<const char *>& extensions)
{
    uint32_t extensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    vector<VkExtensionProperties> available(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, available.data());

    for (const char* extensionName : extensions)
    {
        bool isFound = false;
        for (const auto& extensionProps : available)
        {
            if (strcmp(extensionName, extensionProps.extensionName) == 0)
            {
                isFound = true;
                break;
            }
        }
        if (isFound)
        {
            continue;
        }

        for (const char* layer : validationLayers){

            uint32_t extensionCountLayer;
            vkEnumerateInstanceExtensionProperties(layer, &extensionCountLayer, nullptr);

            vector<VkExtensionProperties> availableLayer(extensionCountLayer);
            vkEnumerateInstanceExtensionProperties(layer, &extensionCountLayer, availableLayer.data());

            for (const auto& extension : availableLayer) {
                if (strcmp(extension.extensionName, extensionName) == 0)
                {
                    isFound = true;
                    break;
                }
            }
            if (isFound)
            {
                break;
            }
        }

        if (!isFound)
        {
            return make_pair(false, extensionName);
        }
    }
    return make_pair(true, "SUCCESS");
}

void VulkanInstance::checkLayersAndExtensionsSupport(const vector<const char *> &validationLayers,
                                                     const vector<const char *> &extensions)
{
    auto layerSupport = checkValidationLayerSupport(validationLayers);
    if (!layerSupport.first) {
        Logger::failure("Instance Layer not supported! Layer: " + layerSupport.second);
    }

    auto extensionSupport = checkExtensionSupport(validationLayers, extensions);
    if (!extensionSupport.first) {
        Logger::failure("Instance extension not supported! Extension: " + extensionSupport.second);
    }
}
