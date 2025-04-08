#include "ThreadGuard.hpp"

namespace utils {

ThreadGuard::ThreadGuard(std::thread &thread) : thread_(thread) {}

ThreadGuard::~ThreadGuard() noexcept {
  if (thread_.joinable()) {
    thread_.join();
  }
}

}  // namespace utils
