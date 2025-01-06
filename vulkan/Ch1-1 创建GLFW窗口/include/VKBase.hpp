//
// Created by whx on 25-1-5.
//

#ifndef VKBASE_HPP
#define VKBASE_HPP

#include "EasyVKstart.hpp"

/*
    一个呈现图像的Vulkan应用程序需经历以下的步骤以初始化:
    1.创建Vulkan实例
    2.创建debug messenger（若编译选项为DEBUG）
    3.创建window surface
    4.选择物理设备并创建逻辑设备，取得队列
    5.创建交换链
*/
namespace vulkan {

class graphicsBase
{
public:
    ~graphicsBase()
    {
        fmt::print(fmt::fg(fmt::color::green),
            "graphicsBase has been destructed!");
    }

    static graphicsBase &GetInstance()
    {
        static graphicsBase instance;
        return instance;
    }

    VkInstance GetVkInstance() const
    {
        return instance_;
    }

    const std::vector<const char *>& GetInstanceLayers() const
    {
        return instanceLayers_;
    }

    const std::vector<const char *>& GetInstanceExtensions() const
    {
        return instanceExtensions_;
    }

    VkSurfaceKHR GetSurface() const
    {
        return surface_;
    }

    void SetInstanceLayers(const std::vector<const char *>& layerNames)
    {
        instanceLayers_ = layerNames;
    }

    void SetInstanceExtensions(const std::vector<const char *>& extensionNames)
    {
        instanceExtensions_ = extensionNames;
    }

    void SetVkSurfaceKHR(const VkSurfaceKHR &surface)
    {
        if(!surface_)
        {
            surface_ = surface;
        }
    }

    void AddInstanceLayer(const char *layerName)
    {
        AddLayerOrExtension(instanceLayers_, layerName);
    }

    void AddInstanceExtension(const char *extensionName)
    {
        AddLayerOrExtension(instanceExtensions_, extensionName);
    }

    VkResult CreateVkInstance(VkInstanceCreateFlags flags = 0)
    {
        // TODO: 创建Vulkan实例
        return VkResult();
    }

    VkResult CheckInstanceLayers(std::span<const char *> layersToCheck) const
    {
        // TODO: 检测层是否可用
        return VkResult();
    }

    VkResult CheckInstanceExtensions(std::span<const char *> extensionsToCheck, const char *layerName = nullptr) const
    {
        // TODO: 检测扩展是否可用
        return VkResult();
    }

public:
    graphicsBase(graphicsBase &&) = delete;
    graphicsBase(const graphicsBase &) = delete;
    graphicsBase& operator=(const graphicsBase &) = delete;
    graphicsBase& operator=(const graphicsBase &&) = delete;

private:
    graphicsBase() = default;

    static void AddLayerOrExtension(std::vector<const char *>& container, const char *name)
    {
        for (const auto &i : container)
            if (!strcmp(name, i))
                return;
        container.push_back(name);
    }

    VkResult CreateDebugMessenger()
    {
        // TODO：创建debugMessenger
        return VkResult();
    }

private:
    VkInstance instance_{};
    std::vector<const char *> instanceLayers_{};
    std::vector<const char *> instanceExtensions_{};

    VkDebugUtilsMessengerEXT debugMessenger_{};

    VkSurfaceKHR surface_{};
};

}

#endif //VKBASE_HPP
