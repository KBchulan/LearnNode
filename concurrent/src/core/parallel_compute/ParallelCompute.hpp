/******************************************************************************
 *
 * @file       ParallelCompute.hpp
 * @brief      函数式编程实现并行计算
 *
 * @author     KBchulan
 * @date       2025/04/21
 * @history
 ******************************************************************************/

#ifndef PARALLEL_COMPUTE_HPP
#define PARALLEL_COMPUTE_HPP

#include <algorithm>
#include <core/CoreExport.hpp>
#include <global/Singleton.hpp>

#include <array>
#include <concepts>
#include <cstddef>
#include <future>
#include <ranges>

namespace core {

class CORE_EXPORT ParallelCompute final
    : public global::Singleton<ParallelCompute> {
public:
  static void enterFunc() noexcept;

private:
  template <typename T, size_t N>
  static void tradition_quick_sort(std::array<T, N> &arr, int start,
                                   int end) noexcept {
    if (start >= end) {
      return;
    }

    int pivot = arr[start];
    int left = start;
    int right = end;

    while (left < right) {
      while (left < right && arr[right] >= pivot) {
        right--;
      }
      while (left < right && arr[left] <= pivot) {
        left++;
      }
      if (left < right) {
        std::swap(arr[left], arr[right]);
      }
    }
    std::swap(arr[left], arr[start]);

    tradition_quick_sort(arr, start, left - 1);
    tradition_quick_sort(arr, right + 1, end);
  }

  template <typename T>
    requires std::ranges::bidirectional_range<T> &&
             std::movable<std::ranges::range_value_t<T>>
  static T parallel_quick_sort(T input) {
    if (input.empty()) {
      return input;
    }

    T result;
    result.emplace_back(input.front());
    input.erase(input.begin());
    const auto &pivot = result.front();

    auto divide_point = std::ranges::partition(
        input, [&](const auto &val) { return val < pivot; });

    T lower_part;
    for (auto it = input.begin(); it != divide_point.begin(); ++it) {
      lower_part.emplace_back(std::move(*it));
    }

    std::future<T> new_lower(
        std::async(&parallel_quick_sort<T>, std::move(lower_part)));

    T higher_part;
    for (auto it = divide_point.begin(); it != input.end(); ++it) {
      higher_part.emplace_back(std::move(*it));
    }
    auto new_higher = parallel_quick_sort(std::move(higher_part));

    auto final_result = new_lower.get();
    final_result.insert(final_result.end(), result.begin(), result.end());
    final_result.insert(final_result.end(), new_higher.begin(),
                        new_higher.end());

    return final_result;
  }
};

} // namespace core

#define parallelCompute core::ParallelCompute::getInstance()

#endif // PARALLEL_COMPUTE_HPP