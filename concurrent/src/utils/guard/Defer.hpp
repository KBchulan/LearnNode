#ifndef DEFER_HPP
#define DEFER_HPP

#include <functional>

namespace utils {

class Defer {
 public:
  template <typename Func>
  explicit Defer(Func &&func) : func_(std::forward<Func>(func)) {}

  ~Defer() noexcept;

  // 禁止拷贝
  Defer(const Defer &) = delete;
  Defer &operator=(const Defer &) = delete;

  Defer(Defer &&other) noexcept;
  Defer &operator=(Defer &&other) noexcept;

 private:
  std::function<void()> func_;
};

}  // namespace utils

#define DEFER_CONCAT_IMPL(x, y) x##y
#define DEFER_CONCAT(x, y) DEFER_CONCAT_IMPL(x, y)
#define defer auto DEFER_CONCAT(defer_, __LINE__) = utils::Defer

#endif  // DEFER_HPP
