#ifndef MATH_SQRT_HPP
#define MATH_SQRT_HPP

#include <cmath>
#include <stdexcept>
#include <type_traits>
#include <concepts>

namespace math
{
  template <typename T>
    requires std::is_arithmetic_v<T>
  decltype(auto) MySqrt(T x, double epsilon = 1e-10, size_t max_iterations = 1000)
  {
    using ResultType = std::conditional_t<std::is_integral_v<T>, double, T>;

    if (x < 0)
    {
      throw std::invalid_argument("Cannot calculate square root of negative number");
    }
    if (x == 0 || x == 1)
    {
      return static_cast<ResultType>(x);
    }

    ResultType guess = static_cast<ResultType>(x) / 2;
    ResultType prev_guess;

    size_t iteration = 0;
    do
    {
      prev_guess = guess;
      guess = (guess + x / guess) / 2;

      if (std::abs(guess - prev_guess) / std::abs(guess) < epsilon)
      {
        return guess;
      }

      if (++iteration >= max_iterations)
      {
        throw std::runtime_error("Failed to converge within maximum iterations");
      }
    } while (true);
  }
} // namespace math

#endif // MATH_SQRT_HPP