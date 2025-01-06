//
// Created by whx on 25-1-5.
//

#ifndef GLFWGENERAL_HPP
#define GLFWGENERAL_HPP

#include "VKBase.hpp"

// 如果要将GLFW用于Vulkan，这是必须的
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

inline GLFWwindow* Window = nullptr;
inline GLFWmonitor* Monitor = nullptr;

constexpr auto windowTitle = "EasyVulkan";
constexpr VkExtent2D FULL_SIZE = {2560, 1440};
constexpr VkExtent2D WINDOW_SIZE = {1280, 720};

inline void MakeWindowFullScreen()
{
    if(!Monitor)
    {
        Monitor = glfwGetPrimaryMonitor();
    }

    const GLFWvidmode* mode = glfwGetVideoMode(Monitor);
    glfwSetWindowMonitor(Window, Monitor, 0, 0,
        mode->width, mode->height, mode->refreshRate);
}

inline void MakeWindowWindowed(const VkExtent2D position, const VkExtent2D size)
{
    if(!Monitor)
    {
        Monitor = glfwGetPrimaryMonitor();
    }

    const GLFWvidmode* mode = glfwGetVideoMode(Monitor);
    glfwSetWindowMonitor(Window, nullptr,
        static_cast<int>(position.width),
        static_cast<int>(position.height),
        static_cast<int>(size.width),
        static_cast<int>(size.height),
        mode->refreshRate);
}

inline void KeyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_ENTER:
                MakeWindowFullScreen();
                break;
            case GLFW_KEY_ESCAPE:
                MakeWindowWindowed({(FULL_SIZE.width - WINDOW_SIZE.width) / 2,
                    (FULL_SIZE.height - WINDOW_SIZE.height) / 2}, WINDOW_SIZE);
                break;
            case GLFW_KEY_Q:
                // TODO: 处理退出逻辑
                fmt::print(fmt::fg(fmt::color::green), "该退出了，但是还没有逻辑\n");
                break;
            default:
                fmt::print(fmt::fg(fmt::color::orange),
                    "现在还没有处理这个的逻辑\n");
                break;
        }
    }
}

inline bool InitializeWindow(
    const VkExtent2D size = WINDOW_SIZE,
    const bool fullScreen = false,
    const bool isResizeAble = true,
    bool limitFrameRate = true)
{
    if (!glfwInit())
    {
        fmt::print(fmt::fg(fmt::color::red), R"(Failed to initialize GLFW.)");
        return false;
    }

    // GLFWmonitor是显示器的句柄，用于获取和管理显示器的相关信息
    Monitor = glfwGetPrimaryMonitor();
    if (!Monitor)
    {
        fmt::print(fmt::fg(fmt::color::red), R"(Failed to get paimary monitor.)");
        return false;
    }

    // GLFWvidmode是一个结构体，用于描述显示器的视频模式，包括分辨率、刷新率和颜色位深等信息
    const GLFWvidmode* Mode = glfwGetVideoMode(Monitor);
    if (!Mode)
    {
        fmt::print(fmt::fg(fmt::color::red), R"(Failed to get mode.)");
        return false;
    }

    // glfwWindowHint是设置窗口属性
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, isResizeAble);

    // window，第四个参数是指定全屏模式下的显示器，空则为窗口，第五个参数则是一个其他窗口的，可用于共享
    if (fullScreen)
    {
        // 全屏设置
        glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
        glfwWindowHint(GLFW_AUTO_ICONIFY, GLFW_FALSE);
        glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

        // 增加透明绘图和焦点
        glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);

        Window = glfwCreateWindow(Mode->width, Mode->height,
            windowTitle, Monitor, nullptr);
    }
    else
    {
        Window = glfwCreateWindow(static_cast<int>(size.width),
            static_cast<int>(size.height), windowTitle, nullptr, nullptr);
    }

    if (!Window)
    {
        fmt::print(fmt::fg(fmt::color::red), R"(Failed to initialize Window.)");
        glfwTerminate();
        return false;
    }
    else
    {
        if (fullScreen)
        {
            // 创建窗口后立即清除背景
        }
        else
        {
            // 居中显示窗口
            glfwSetWindowMonitor(Window, nullptr,
                static_cast<int>(FULL_SIZE.width - WINDOW_SIZE.width) / 2,
                static_cast<int>(FULL_SIZE.height - WINDOW_SIZE.height) / 2,
                static_cast<int>(size.width),
                static_cast<int>(size.height),
                Mode->refreshRate);
        }
    }

    // 函数回调注册
    glfwSetKeyCallback(Window, KeyCallBack);

    // TODO: 后续补充
    return true;
}

inline void TerminateWindow()
{
    glfwTerminate();
    // TODO: 后续补充
}

inline void TitleFPS()
{
    // glfwGetTime返回自GLFW初始化以来经过的时间, 单位是秒(double类型)
    static double start_time = glfwGetTime();
    static double end_time;
    static double delta_time;
    static std::uint32_t dframe = -1;
    static std::stringstream info;

    end_time = glfwGetTime();
    dframe++;

    if ((delta_time = end_time - start_time) >= 1)
    {
        info.precision(1);
        info << windowTitle << "    " << std::fixed << dframe / delta_time << "FPS";
        glfwSetWindowTitle(Window, info.str().c_str());

        // 回滚数据
        info.str("");
        start_time = end_time;
        dframe = 0;
    }
}

#endif //GLFWGENERAL_HPP
