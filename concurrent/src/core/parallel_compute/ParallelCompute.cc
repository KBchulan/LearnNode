#include "ParallelCompute.hpp"

#include <middleware/Logger.hpp>

namespace core {

void ParallelCompute::enterFunc() noexcept {
  logger.info("hello, parallel");
}

} // namespace core