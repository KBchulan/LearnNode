#ifndef MATH_TEST_HPP
#define MATH_TEST_HPP

#include <middleware/Logger.hpp>

namespace math
{
  void math_test()
  {
    Logger.error("i am math test, {}", 222);
  }
}
#endif // MATH_TEST_HPP
