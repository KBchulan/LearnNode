#include "test.hpp"

namespace utils
{
  void utils_test()
  {
    Logger.fatal("i am a export utils, {}", 333);
  }

  void utils_no_test()
  {
    Logger.fatal("i am a no export utils, {}", 444);
  }
} // namespace utils