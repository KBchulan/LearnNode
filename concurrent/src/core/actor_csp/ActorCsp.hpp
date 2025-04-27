/******************************************************************************
 *
 * @file       ActorCsp.hpp
 * @brief      Actor和Csp两种设计模式
 *
 * @author     KBchulan
 * @date       2025/04/27
 * @history
 ******************************************************************************/

#ifndef ACTOR_CSP_HPP
#define ACTOR_CSP_HPP

#include <core/CoreExport.hpp>
#include <global/Singleton.hpp>

namespace core {

class CORE_EXPORT ActorCsp final : public global::Singleton<ActorCsp> {
public:
  static void enterFunc() noexcept;

private:
  static void actorDesign() noexcept;

  static void cspDesign() noexcept;
};

} // namespace core

#define actorCsp core::ActorCsp::getInstance()

#endif // ACTOR_CSP_HPP