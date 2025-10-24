/******************************************************************************
 *
 * @file       ThreadBasic.hpp
 * @brief      线程相关的基础知识
 * @note       创建，join, detach, 异常处理，auto_guard, jthread， 传入的为仿函数， 引用， unique_ptr, 类的成员函数， 静态成员函数
 *
 * @author     KBchulan
 * @date       2025/04/06
 * @history
 ******************************************************************************/

#ifndef THREAD_BASIC_HPP
#define THREAD_BASIC_HPP

#include <string>
#include <cstdint>

#include <core/CoreExport.hpp>
#include <global/Singleton.hpp>

namespace core {

class CORE_EXPORT ThreadBasic final : public global::Singleton<ThreadBasic> {
 public:
  static void enterFunc() noexcept;

 private:
  static void thread_work_1(std::string &&) noexcept;

  static void oops() noexcept;

  static void catch_exception();

  static void auto_guard() noexcept;

  static void jthreadDemo() noexcept;

  static void danger_oops(std::uint32_t params) noexcept;

  static void safe_oops(std::uint32_t params) noexcept;

  static void ref_oops(std::uint32_t &params) noexcept;

  static void class_oops() noexcept;

  static void unique_oops() noexcept;

  class background_tast {
   public:
    void operator()(std::string &&str);
  };

  struct func {
    std::uint32_t &num_;
    func(std::uint32_t &num);

    void operator()() noexcept;
  };

  class test {
   public:
    void ppp(int &num) noexcept;
    static void print() noexcept;
   private:
    int num_ = 6;
  };
};

}  // namespace core

#define threadBasic core::ThreadBasic::getInstance()

#endif  // THREAD_BASIC_HPP