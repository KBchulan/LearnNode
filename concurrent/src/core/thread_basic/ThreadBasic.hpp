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
  static void CORE_NO_EXPORT thread_work_1(std::string &&) noexcept;

  static void CORE_NO_EXPORT oops() noexcept;

  static void CORE_NO_EXPORT catch_exception();

  static void CORE_NO_EXPORT auto_guard() noexcept;

  static void CORE_NO_EXPORT jthreadDemo() noexcept;

  static void CORE_NO_EXPORT danger_oops(std::uint32_t params) noexcept;

  static void CORE_NO_EXPORT safe_oops(std::uint32_t params) noexcept;

  static void CORE_NO_EXPORT ref_oops(std::uint32_t &params) noexcept;

  static void CORE_NO_EXPORT class_oops() noexcept;

  static void CORE_NO_EXPORT unique_oops() noexcept;

  class CORE_NO_EXPORT background_tast {
   public:
    void operator()(std::string &&str);
  };

  struct CORE_NO_EXPORT func {
    std::uint32_t &num_;
    func(std::uint32_t &num);

    void operator()() noexcept;
  };

  class CORE_NO_EXPORT test {
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