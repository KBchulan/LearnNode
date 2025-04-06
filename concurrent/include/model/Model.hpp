/******************************************************************************
 *
 * @file       Model.hpp
 * @brief      数据结构的集中声明
 *
 * @author     KBchulan
 * @date       2025/04/03
 * @history
 ******************************************************************************/

#ifndef MODEL_HPP
#define MODEL_HPP

#include <cstdint>
#include <string>

namespace model {

struct RedisConfig {
  std::string host;
  std::uint32_t port;
  std::string password;
};

}  // namespace model

#endif  // MODEL_HPP