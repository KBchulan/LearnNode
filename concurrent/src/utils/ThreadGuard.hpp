/******************************************************************************
 *
 * @file       ThreadGuard.hpp
 * @brief      自动守卫的线程
 *
 * @author     KBchulan
 * @date       2025/04/08
 * @history
 ******************************************************************************/

#ifndef THREADGUARD_HPP
#define THREADGUARD_HPP

#include <thread>
#include <utils/UtilsExport.hpp>

namespace utils {

class UTILS_EXPORT ThreadGuard {
 public:
  explicit ThreadGuard(std::thread &thread);

  ~ThreadGuard() noexcept;

  ThreadGuard(const ThreadGuard &) = delete;
  ThreadGuard &operator=(const ThreadGuard &) = delete;

 private:
  std::thread &thread_;
};

}  // namespace utils

#endif  // THREADGUARD_HPP