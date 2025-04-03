#ifndef CORE_TEST_HPP
#define CORE_TEST_HPP

#include "CoreExport.hpp"

#include "../../include/middleware/Logger.hpp"

namespace core
{
  void CORE_EXPORT core_test()
  {
    Logger.debug("i am core test, {}", 111);
  }
} // namespace core

#endif // CORE_TEST_HPP
