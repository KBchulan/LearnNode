#ifndef EASYVKSTART_HPP
#define EASYVKSTART_HPP

// cc standard
#include <map>
#include <stack>
#include <tuple>
#include <vector>
#include <unordered_map>

#include <span>
#include <memory>
#include <chrono>

#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

#include <numbers>
#include <numeric>
#include <concepts>
#include <functional>

// glm
// NDC深度在vulkan中为[0, 1]，需要强制指定
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// third party
#include <fmt/core.h>
#include <fmt/color.h>
#include <fmt/ranges.h>

#include <stb_image.h>

// vulkan
#include <vulkan/vulkan.h>

// 通用函数
template<typename ReturnValue, typename... Args>
class RAII
{
public:
    explicit RAII(std::function<ReturnValue(Args...)> func) : _func(func){}

    ~RAII()
    {
        std::apply(_func, _args);
        fmt::print(fmt::fg(fmt::color::green), "参数为{},已释放", _args);
    }

    void setArgs(Args... args)
    {
        _args = std::make_tuple(std::forward<Args>(args)...);
    }

private:
    std::tuple<Args...> _args;
    std::function<ReturnValue(Args...)> _func;
};

#endif // !EASYVKSTART_HPP
