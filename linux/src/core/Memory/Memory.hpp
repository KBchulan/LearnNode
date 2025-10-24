/******************************************************************************
 *
 * @file       Memory.hpp
 * @brief      Linux 文件系统的API
 *
 * @author     KBchulan
 * @date       2025/04/04
 * @history
 ******************************************************************************/

#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <string>
#include <CoreExport.hpp>
#include <global/Singleton.hpp>

namespace core {

class CORE_EXPORT Memory final : public global::Singleton<Memory> {
public:
  static void CORE_EXPORT brkCall() noexcept;

  static void CORE_EXPORT mmapCall() noexcept;

private:
  static std::string CORE_NO_EXPORT ptrToString(void *ptr) noexcept;
};

} // namespace core

#define memory core::Memory::getInstance()

#endif // MEMORY_HPP
