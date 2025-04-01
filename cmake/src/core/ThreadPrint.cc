#include "ThreadPrint.hpp"

core::ThreadPrint::ThreadPrint()
{
  std::vector<std::uint32_t> nums = {1, 2, 3, 4};

  std::thread thr(
    [&](std::span<std::uint32_t> args) -> void
    {
      std::uint32_t sum = 0;
      for (const auto &a : args)
        sum += a;
      std::puts(std::to_string(sum).c_str());
    },
    std::span<std::uint32_t>(nums)
  );

  thr.join();
}

void core::ThreadPrint::print()
{
  std::puts("hello core");
}