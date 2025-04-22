/******************************************************************************
 *
 * @file       AsyncFunc.hpp
 * @brief      并发三剑客以及线程池的创建
 *
 * @author     KBchulan
 * @date       2025/04/21
 * @history
 ******************************************************************************/

#ifndef ASYNCFUNC_HPP
#define ASYNCFUNC_HPP

#include <core/CoreExport.hpp>
#include <global/Singleton.hpp>

namespace core {

class CORE_EXPORT AsyncFunc final : public global::Singleton<AsyncFunc> {
public:
  static void enterFunc() noexcept;

private:
  static void futureCall() noexcept;

  static void packageCall() noexcept;

  static void promiseCall() noexcept;

  static void exceptionCall() noexcept;

  static void destructCall() noexcept;
};

} // namespace core

#define asyncFunc core::AsyncFunc::getInstance()

#endif // ASYNCFUNC_HPP