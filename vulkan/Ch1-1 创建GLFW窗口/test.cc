#include <array>
#include <cstdint>
#include <fmt/color.h>
#include <vector>
#include <span>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <stdint.h>
#include <iostream>
#include <algorithm>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    auto print_info = [](std::span<std::uint32_t> data) -> void
    {
        fmt::print("data: {}\n", data);
    };

    std::vector<std::uint32_t> nums = {1, 2, 6, 3, 4, 5};
    std::span<std::uint32_t> sp = nums;
    std::array<std::uint32_t, 5> arr = {1, 2, 3, 4, 5};
    print_info(sp);
    print_info(arr);

    nums.erase(std::remove_if(nums.begin(),
        nums.end(), [](std::uint32_t num) ->bool
        {
            return num > 3;
        }), nums.end());
}
