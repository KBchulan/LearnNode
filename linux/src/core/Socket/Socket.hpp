/******************************************************************************
 *
 * @file       Socket.hpp
 * @brief      Linux 网络管理的api
 *
 * @author     KBchulan
 * @date       2025/04/06
 * @history
 ******************************************************************************/

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <CoreExport.hpp>
#include <global/Singleton.hpp>

namespace core {

class CORE_EXPORT SocketCall final : public global::Singleton<SocketCall> {
public:
  static void CORE_EXPORT socketClientCall() noexcept;

  static void CORE_EXPORT socketServerCall() noexcept;

};

}  // namespace core

#define socketCall core::SocketCall::getInstance()

#endif  // SOCKET_HPP
