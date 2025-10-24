/******************************************************************************
 *
 * @file       AtomicMemory.hpp
 * @brief      基本的原子操作和内存序的介绍
 *
 * @author     KBchulan
 * @date       2025/04/29
 * @history
 ******************************************************************************/

#ifndef ATOMIC_MEMORY_HPP
#define ATOMIC_MEMORY_HPP

#include <global/Singleton.hpp>
#include <core/CoreExport.hpp>

namespace core {

class CORE_EXPORT AtomicMemory final : public global::Singleton<AtomicMemory> {
public:
  static void enterFunc() noexcept;

private:
  static void atomicFunc() noexcept;

  static void relaxedFunc() noexcept;

  static void seqConsFunc() noexcept;

  static void acqrelFunc() noexcept;
};

} // namespace core

#define atomicMemory core::AtomicMemory::getInstance()

#endif // ATOMIC_MEMORY_HPP