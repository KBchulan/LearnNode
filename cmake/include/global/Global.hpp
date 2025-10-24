/******************************************************************************
 *
 * @file       Global.hpp
 * @brief      这里写全局的声明配置
 *
 * @author     KBchulan
 * @date       2025/04/03
 * @history
 ******************************************************************************/

#ifndef GLOBAL_HPP
#define GLOBAL_HPP

#include <functional>
#include <global/Singleton.hpp>
#include <model/Model.hpp>
#include <thread>

namespace global {

class GlobalVariable final : public Singleton<GlobalVariable> {
 public:
  std::hash<std::thread::id> hasher{};
};

}  // namespace global

#define globalVariable global::GlobalVariable::getInstance()

#endif  // GLOBAL_HPP
