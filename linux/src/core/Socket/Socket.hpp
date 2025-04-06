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

class CORE_EXPORT Socket final : public global::Singleton<Socket> {
public:
  static void socketCall() noexcept;
};

}  // namespace core

#define socket core::Socket::getInstance()

#endif  // SOCKET_HPP
