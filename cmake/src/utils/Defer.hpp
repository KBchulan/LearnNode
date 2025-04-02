#ifndef UTILS_DEFER_HPP
#define UTILS_DEFER_HPP

#include <functional>

#include "utils_export.hpp"

namespace utils
{
  class UTILS_EXPORT Defer
  {
  public:
    explicit Defer(std::function<void()> callback);

    void UTILS_EXPORT visiable();

    void UTILS_NO_EXPORT unvisiable();

    ~Defer();

  private:
    std::function<void()> callback_;
  };
};

#endif // !UTILS_DEFER_HPP