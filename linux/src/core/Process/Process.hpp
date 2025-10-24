/******************************************************************************
 *
 * @file       Process.hpp
 * @brief      Linux 进程管理的api
 *
 * @author     KBchulan
 * @date       2025/04/04
 * @history
 ******************************************************************************/

#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <CoreExport.hpp>
#include <global/Singleton.hpp>

namespace core {

class CORE_EXPORT Process final : public global::Singleton<Process> {
public:
  static void CORE_EXPORT forkCall() noexcept;

  static void CORE_EXPORT sched_yieldCall() noexcept;

  static void CORE_EXPORT shmCall() noexcept;
};

} // namespace core

#define process core::Process::getInstance()

#endif // PROCESS_HPP
