/******************************************************************************
 *
 * @file       Cache.hpp
 * @brief      实现缓存的管理
 *
 * @author     KBchulan
 * @date       2025/04/03
 * @history
 ******************************************************************************/

#ifndef CACHE_HPP
#define CACHE_HPP

#include <global/Singleton.hpp>

namespace middleware {

class Cache final : public global::Singleton<Cache> {};

}  // namespace middleware

#define cache middleware::Cache::getInstance()

#endif  // CACHE_HPP