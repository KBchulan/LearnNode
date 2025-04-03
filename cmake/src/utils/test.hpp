#ifndef UTILS_TEST_HPP
#define UTILS_TEST_HPP

#include "UtilsExport.hpp"

#include "../../include/middleware/Logger.hpp"

namespace utils
{
  void UTILS_EXPORT utils_test()
  {
    Logger.fatal("i am utils, {}", 333);
  }
} // namespace utils

#endif // UTILS_TEST_HPP
