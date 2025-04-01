#ifndef UTILS_DEFER_HPP
#define UTILS_DEFER_HPP

#include <functional>

namespace utils
{
  class Defer
  {
  public:
    explicit Defer(std::function<void()> callback);

    ~Defer();

  private:
    std::function<void()> callback_;
  };
};

#endif // !UTILS_DEFER_HPP