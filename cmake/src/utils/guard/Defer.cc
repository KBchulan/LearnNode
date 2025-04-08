#include "Defer.hpp"

#include <utility>

namespace utils {

Defer::Defer(Defer &&other) noexcept
    : func_(std::exchange(other.func_, nullptr)) {}

Defer &Defer::operator=(Defer &&other) noexcept {
  func_ = std::exchange(other.func_, nullptr);
  return *this;
}

Defer::~Defer() noexcept {
  if (func_) {
    func_();
  }
}

}  // namespace utils