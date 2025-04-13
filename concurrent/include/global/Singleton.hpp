/******************************************************************************
 *
 * @file       Singleton.hpp
 * @brief      单例模板的实现
 *
 * @author     KBchulan
 * @date       2025/04/03
 * @history
 ******************************************************************************/

#ifndef SINGLETON_HPP
#define SINGLETON_HPP

namespace global {

template <typename T>
class Singleton {
 public:
  static T &getInstance() noexcept {
    static T instance;
    return instance;
  }

  Singleton(const Singleton &) = delete;
  Singleton &operator=(const Singleton &) = delete;

  Singleton(const Singleton &&) = delete;
  Singleton &operator=(const Singleton &&) = delete;

 protected:
  Singleton() = default;
  ~Singleton() = default;
};

}  // namespace global

#endif  // SINGLETON_HPP