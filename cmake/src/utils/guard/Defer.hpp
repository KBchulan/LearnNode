#ifndef DEFER_HPP
#define DEFER_HPP

#include <functional>
#include <utils/UtilsExport.hpp>

namespace utils {

class UTILS_EXPORT Defer {
 public:
  template <typename Func>
  explicit Defer(Func &&func) : func_(std::forward<Func>(func)) {}

  ~Defer() noexcept;

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
