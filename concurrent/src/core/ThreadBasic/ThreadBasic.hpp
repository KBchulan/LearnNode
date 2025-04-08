/******************************************************************************
 *
 * @file       ThreadBasic.hpp
 * @brief      线程相关的基础知识
 *
 * @author     KBchulan
 * @date       2025/04/06
 * @history
 ******************************************************************************/

#ifndef THREAD_BASIC_HPP
#define THREAD_BASIC_HPP

#include <core/CoreExport.hpp>
#include <global/Singleton.hpp>
#include <string>

namespace core {

class CORE_EXPORT ThreadBasic final : public global::Singleton<ThreadBasic> {
 public:
  static void basic() noexcept;

 private:
  static void thread_work_1(std::string &&) noexcept;

  static void oops() noexcept;

  static void catch_exception();

  static void auto_guard() noexcept;

  static void jthreadDemo() noexcept;

  class background_tast {
   public:
    void operator()(std::string &&str);
  };

  struct func {
    std::uint32_t &num_;
    func(std::uint32_t &num);

    void operator()() noexcept;
  };
};

}  // namespace core

#define threadBasic core::ThreadBasic::getInstance()

#endif  // THREAD_BASIC_HPP