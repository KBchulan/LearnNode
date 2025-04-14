/******************************************************************************
 *
 * @file       ThreadManage.hpp
 * @brief      本章介绍一下线程管控，即线程的归属权
 *
 * @author     KBchulan
 * @date       2025/04/12
 * @history
 ******************************************************************************/

#ifndef THREAD_MANAGE_HPP
#define THREAD_MANAGE_HPP

#include <algorithm>
#include <iterator>
#include <numeric>
#include <thread>
#include <vector>

#include <core/CoreExport.hpp>
#include <global/Singleton.hpp>

namespace core {

class CORE_EXPORT ThreadManage final : public global::Singleton<ThreadManage> {
 public:
  static void enterFunc() noexcept;

 private:
  static void CORE_NO_EXPORT dangerousUse() noexcept;

  static std::thread CORE_NO_EXPORT rvoAndnrvoUse() noexcept;

  static void CORE_NO_EXPORT jthreadUse() noexcept;

  static void CORE_NO_EXPORT vectorUse() noexcept;

  static void CORE_NO_EXPORT accUse() noexcept;

  static void CORE_NO_EXPORT threadInfo() noexcept;

  template <typename Iterator, typename T>
  static T CORE_NO_EXPORT parrelAcc(Iterator first, Iterator last, T init) noexcept {
    const auto length = std::distance(first, last);
    if (length == 0) {
      return init;
    }

    const unsigned int num_threads = std::min(std::thread::hardware_concurrency(), static_cast<unsigned int>((length + 24) / 25));

    if (num_threads <= 1) {
      return std::accumulate(first, last, init);
    }

    std::vector<T> results(num_threads);
    std::vector<std::thread> threads;
    threads.reserve(num_threads - 1);

    const auto chunk_size = length / num_threads;
    const auto remainder = length % num_threads;

    auto process_chunk = [](Iterator start, Iterator end, T& result) {
      result = std::accumulate(start, end, T{});
    };

    auto chunk_begin = first;
    for (unsigned int i = 0; i < num_threads - 1; ++i) {
      auto extra = (i < remainder) ? 1 : 0;
      auto chunk_end = chunk_begin;
      std::advance(chunk_end, chunk_size + extra);

      threads.emplace_back(process_chunk, chunk_begin, chunk_end, std::ref(results[i]));
      chunk_begin = chunk_end;
    }
    for(auto& thread: threads) {
      thread.join();
    }

    // 主线程处理最后一块
    results[num_threads - 1] = std::accumulate(chunk_begin, last, T{});

    // 汇总结果
    return std::accumulate(results.begin(), results.end(), init);
  }
};

}  // namespace core

#define threadManage core::ThreadManage::getInstance()

#endif  // THREAD_MANAGE_HPP
