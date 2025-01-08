//
// Created by whx on 25-1-5.
//

#ifndef VKBASE_HPP
#define VKBASE_HPP

#include "EasyVKstart.hpp"

#include <algorithm>
#include <cstdint>
#include <vector>
#include <vulkan/vulkan_core.h>

/*
    一个呈现图像的Vulkan应用程序需经历以下的步骤以初始化:
    1.创建Vulkan实例
    2.创建debug messenger（若编译选项为DEBUG）
    3.创建window surface
    4.选择物理设备并创建逻辑设备，取得队列(Vulkan queues provide an interface to the execution engines of a device.)
    5.创建交换链
*/
namespace vulkan {

class graphicsBase
{
public:
    ~graphicsBase()
    {
        fmt::print(fmt::fg(fmt::color::green),
            "graphicsBase has been destructed!\n");
    }

    /* ---------------------------------------------------------------------------- */
    // Getter
    static graphicsBase &GetInstance()
    {
        static graphicsBase instance;
        return instance;
    }

    VkInstance GetVkInstance() const
    {
        return instance_;
    }

    const std::vector<const char *> &GetInstanceLayers() const
    {
        return instanceLayers_;
    }

    const std::vector<const char *> &GetInstanceExtensions() const
    {
        return instanceExtensions_;
    }

    VkSurfaceKHR GetSurface() const
    {
        return surface_;
    }

    VkPhysicalDevice GetPhysicalDevice() const
    {
        return physicalDevice_;
    }

    const VkPhysicalDeviceProperties &GetPhysicalDeviceProperties() const
    {
        return physicalDeviceProperties_;
    }

    const VkPhysicalDeviceMemoryProperties &GetPhysicalDeviceMemoryProperties() const
    {
        return physicalDeviceMemoryProperties_;
    }

    VkPhysicalDevice AvailablePhysicalDevice(std::uint32_t index) const
    {
        return availablePhysicalDevices_[index];
    }

    VkDevice Device() const
    {
        return device_;
    }

    std::uint32_t QueueFamilyIndexGraphics() const
    {
        return queueFamilyIndex_graphics_;
    }

    std::uint32_t QueueFamilyIndexPresentation() const
    {
        return queueFamilyIndex_presentation_;
    }

    std::uint32_t QueueFamilyIndexCompute() const
    {
        return queueFamilyIndex_compute_;
    }

    VkQueue QueueGraphics() const
    {
        return queue_graphics_;
    }

    VkQueue QueuePresentation() const
    {
        return queue_presentation_;
    }

    VkQueue QueueCompute() const
    {
        return queue_compute_;
    }

    const std::vector<const char *> &DeviceExtensions() const
    {
        return deviceExtensions_;
    }

    const VkFormat &GetAvailableSurfaceFormat(std::uint32_t index) const
    {
        return availableSurfaceFormats_[index].format;
    }

    const VkColorSpaceKHR &GetAvailableSurfaceColorSpace(uint32_t index) const
    {
        return availableSurfaceFormats_[index].colorSpace;
    }

    uint32_t GetAvailableSurfaceFormatCount() const
    {
        return uint32_t(availableSurfaceFormats_.size());
    }

    VkSwapchainKHR GetSwapchain() const
    {
        return swapchain_;
    }
    VkImage GetSwapchainImage(uint32_t index) const
    {
        return swapchainImages_[index];
    }
    VkImageView GetSwapchainImageView(uint32_t index) const
    {
        return swapchainImageViews_[index];
    }
    uint32_t GetSwapchainImageCount() const
    {
        return uint32_t(swapchainImages_.size());
    }
    const VkSwapchainCreateInfoKHR& GetSwapchainCreateInfo() const
    {
        return swapchainCreateInfo_;
    }

    /* ---------------------------------------------------------------------------- */
    // Setter
    void SetInstanceLayers(const std::vector<const char *> &layerNames)
    {
        instanceLayers_ = layerNames;
    }

    void SetInstanceExtensions(const std::vector<const char *> &extensionNames)
    {
        instanceExtensions_ = extensionNames;
    }

    void SetVkSurfaceKHR(const VkSurfaceKHR &surface)
    {
        if(!this->surface_)
            surface_ = surface;
    }

    void SetDeviceExtensions(const std::vector<const char *> &extensionNames)
    {
        deviceExtensions_ = extensionNames;
    }

    /* ---------------------------------------------------------------------------- */
    // 创建vulkan实例
    VkResult CreateVkInstance(VkInstanceCreateFlags flags = 0)
    {
    // TODO: 创建Vulkan实例
    #ifdef NOEBUG
        AddInstanceLayer("VK_LAYER_KHRONOS_validation");
        AddInstanceExtension(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    #endif
        VkApplicationInfo applicationInfo =
        {
            .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .apiVersion = apiVersion
        };
        VkInstanceCreateInfo instanceCreateInfo =
        {
            .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
            .flags = flags,
            .pApplicationInfo = &applicationInfo,
            .enabledLayerCount = std::uint32_t(instanceLayers_.size()),

        };
    }

    void AddInstanceLayer(const char *layerName)
    {
        AddLayerOrExtension(instanceLayers_, layerName);
    }

    void AddInstanceExtension(const char *extensionName)
    {
        AddLayerOrExtension(instanceExtensions_, extensionName);
    }

    VkResult CheckInstanceLayers(std::span<const char *> layersToCheck) const
    {
        // TODO: 检测实例层是否可用
        return VkResult();
    }

    VkResult CheckInstanceExtensions(std::span<const char *> extensionsToCheck, const char *layerName = nullptr) const
    {
        // TODO: 检测实例扩展是否可用
        return VkResult();
    }

    void AddDeviceExtension(const char *extensionName)
    {
        AddLayerOrExtension(deviceExtensions_, extensionName);
    }

    VkResult GetPhysicalDevices()
    {
        // TODO: 获取物理设备
        return VkResult();
    }

    // 决定物理设备并调用GetQueueFamilyIndices
    VkResult DeterminePhysicalDevice(std::uint32_t deviceIndex = 0, bool enableGraphicsQueue = true, bool enableComputeQueue = true)
    {
        // TODO: 选择物理设备
        return VkResult();
    }

    VkResult CreateDevice(VkDeviceCreateFlags flags = 0)
    {
        // TODO: 创建逻辑设备
        return VkResult();
    }

    VkResult CheckDeviceExtensions(std::span<const char *> extensionsToCheck) const
    {
        // TODO: 检测设备扩展是否可用
        return VkResult();
    }

    VkResult GetSurfaceFormats()
    {
        // TODO: 获取表面格式
        return VkResult();
    }

    VkResult SetSurfaceFormat(VkSurfaceFormatKHR surfaceFormat)
    {
        // TODO: 设置表面格式
        return VkResult();
    }

    //该函数用于创建交换链
    VkResult CreateSwapchain(bool limitFrameRate = true, VkSwapchainCreateFlagsKHR flags = 0)
    {
        // TODO: 创建交换链
        return VkResult();
    }

    //该函数用于重建交换链
    VkResult RecreateSwapchain()
    {
        // TODO: 重建交换链
        return VkResult();
    }

public:
    graphicsBase(graphicsBase &&) = delete;
    graphicsBase(graphicsBase &) = delete;
    graphicsBase& operator=(graphicsBase &) = delete;
    graphicsBase& operator=(graphicsBase &&) = delete;

private:
    graphicsBase() = default;

    void AddLayerOrExtension(std::vector<const char *> &container, const char *name)
    {
        if (std::find(container.begin(), container.end(), name) != container.end())
            return;
        container.push_back(name);
    }

    VkResult CreateDebugMessenger()
    {
        // TODO：创建debugMessenger
        return VkResult();
    }

    VkResult GetQueueFamilyIndices(VkPhysicalDevice physicalDevice,
        bool enableGraphicsQueue = true, bool enableComputeQueue = true)
    {
        return VkResult();
    }

    // 保存交换链的创建信息便于重建
    VkSwapchainCreateInfoKHR swapchainCreateInfo_ = {};

    VkResult CreateSwapchain_internal()
    {
        return VkResult();
    }

private:
    // api版本
    std::uint32_t apiVersion = VK_API_VERSION_1_0;

    // vulkan实例
    VkInstance instance_{}; 

    // debug messenger
    VkDebugUtilsMessengerEXT debugMessenger_{};

    // 窗口表面，用于Vulkan与窗口系统交互，相当于一个画布
    VkSurfaceKHR surface_{};

    // 物理设备的获取
    VkPhysicalDevice physicalDevice_{};
    VkPhysicalDeviceProperties physicalDeviceProperties_{};
    VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties_{};

    // 创建逻辑设备
    VkDevice device_{};

    // 队列族索引，graphics是绘图队列(渲染)，presentation是呈现队列(与屏幕交换)，compute是计算队列(着色器)
    std::uint32_t queueFamilyIndex_graphics_ = VK_QUEUE_FAMILY_IGNORED;
    std::uint32_t queueFamilyIndex_presentation_ = VK_QUEUE_FAMILY_IGNORED;
    std::uint32_t queueFamilyIndex_compute_ = VK_QUEUE_FAMILY_IGNORED;

    // 队列族
    VkQueue queue_graphics_{};
    VkQueue queue_presentation_{};
    VkQueue queue_compute_{};

    // 创建交换链
    VkSwapchainKHR swapchain_{};
    std::vector<VkImage> swapchainImages_{};
    std::vector<VkImageView> swapchainImageViews_{};

    // vector集合
    std::vector<const char *> instanceLayers_{};
    std::vector<const char *> instanceExtensions_{};
    std::vector<const char *> deviceExtensions_{};

    // 描述表面的格式信息，可用的物理设备
    std::vector<VkSurfaceFormatKHR> availableSurfaceFormats_{};
    std::vector<VkPhysicalDevice> availablePhysicalDevices_{};
};

}

#endif //VKBASE_HPP
