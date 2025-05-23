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

class CORE_EXPORT MutexDeadLock final
    : public global::Singleton<MutexDeadLock> {
 public:
  static void enterFunc() noexcept;

 private:
  static void mutexCall() noexcept;

  static void rwmutexCall() noexcept;

  static void recursivemutexCall() noexcept;

  static void conditionVariableCall() noexcept;

  static void spinlockCall() noexcept;

  static void semaphoreCall() noexcept;

  static void atomicCall() noexcept;

  static void timedCall() noexcept;

  static void deadLockCall() noexcept;

  static void lockGuardCall() noexcept;

  static void uniqueLockCall() noexcept;

  static void scopedLockCall() noexcept;

  static void sharedLockCall() noexcept;
};

}  // namespace core

#define mutexDeadLock core::MutexDeadLock::getInstance()

#endif  // MUTEX_HPP
