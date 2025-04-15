/******************************************************************************
 *
 * @file       Mutex.hpp
 * @brief      这一章介绍一下互斥和死锁
 *
 * @author     KBchulan
 * @date       2025/04/13
 * @history
 ******************************************************************************/

#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <core/CoreExport.hpp>
#include <global/Singleton.hpp>

namespace core {

class CORE_EXPORT MutexDeadLock final : public global::Singleton<MutexDeadLock> {
 public:
  static void enterFunc() noexcept;

private:
  static void CORE_NO_EXPORT mutexCall() noexcept;

  static void CORE_NO_EXPORT rwmutexCall() noexcept;

  static void CORE_NO_EXPORT recursivemutexCall() noexcept;

  static void CORE_NO_EXPORT conditionVariableCall() noexcept;

  static void CORE_NO_EXPORT lockGuardCall() noexcept;

  static void CORE_NO_EXPORT uniqueLockCall() noexcept;

  static void CORE_NO_EXPORT scopedLockCall() noexcept;

  static void CORE_NO_EXPORT sharedLockCall() noexcept;

};

}  // namespace core

#define mutexDeadLock core::MutexDeadLock::getInstance()

#endif  // MUTEX_HPP
