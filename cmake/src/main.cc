#include "core/test.hpp"
#include "math/test.hpp"
#include "utils/test.hpp"

#include "../include/middleware/Logger.hpp"

int main()
{
  Logger.showExample();
  
  core::core_test();
  math::math_test();
  utils::utils_test();
}
