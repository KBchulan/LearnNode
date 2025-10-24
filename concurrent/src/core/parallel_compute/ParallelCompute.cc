#include "ParallelCompute.hpp"

#include <list>
#include <algorithm>

#include <middleware/Logger.hpp>

namespace core {

void ParallelCompute::enterFunc() noexcept {
  std::array<int, 6> nums = {6, 5, 4, 3, 5, 1};
  tradition_quick_sort(nums, 0, nums.size() - 1);
  logger.info("the sorted result is: {}", nums);

  std::list<int> lists = {5, 1, 3, 6, 4};
  std::ranges::partition(lists, [](auto val) { return val < 4; });
  logger.info("the result is: {}", lists);

  auto result = parallel_quick_sort<std::list<int>>(lists);
  logger.info("the sort result is: {}", result);
}

} // namespace core