/******************************************************************************
 *
 * @file       threadBasic.hpp
 * @brief      XXXX Function
 *
 * @author     KBchulan
 * @date       2025/04/06
 * @history
 ******************************************************************************/

#ifndef THREAD_BASIC_HPP
#define THREAD_BASIC_HPP

#include <string>
#include <CoreExport.hpp>
#include <global/Singleton.hpp>

namespace core {

class CORE_EXPORT ThreadBasic final : public global::Singleton<ThreadBasic> {
 public:
  static void CORE_EXPORT basic() noexcept;

 private:
  static void CORE_NO_EXPORT thread_work_1(std::string &&) noexcept;
};

}  // namespace core

#define threadBasic core::ThreadBasic::getInstance()

#endif  // THREAD_BASIC_HPP