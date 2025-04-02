#include <memory>
#include <iostream>

#include "core/ThreadPrint.hpp"
#include "utils/Defer.hpp"

#ifdef USE_MYMATH
 #include "math/Sqrt.hpp"
#else
 #include <cmath>
#endif

#include "../include/config/config.hpp"

int main(int argc, char *argv[])
{
  std::unique_ptr<core::ThreadPrint> ptr = std::make_unique<core::ThreadPrint>();
  ptr->print();
  
  utils::Defer defer(
    [&]() -> void
    {
      // print define
      std::puts("CMAKE Name is: " PROJECT_NAME);
      std::puts("CMAKE Version is: " PROJECT_VERSION);

      // print argc, argv
      std::puts(("argc is: " + std::to_string(argc)).c_str());
      for (int i = 0; i < argc; i++)
      {
        std::puts(("argument is: " + std::string(argv[i])).c_str());
      }

      // sqrt
      #ifdef USE_MYMATH
        std::cout << "mysqrt result is: " << math::MySqrt(argc > 1 ? std::stoi(argv[1]) : 9) << '\n';
      #else
        std::cout << "sqrt result is: " << std::sqrt(argc > 1 ? std::stoi(argv[1]) : 9) << '\n';
      #endif

      // log
      #if defined(HAVE_LOG)
        std::puts("this system has log func");
      #else
        std::puts("this system doesn't has log func");
      #endif

      // after thread
      std::puts("all resources were released!");
    }
  );

  std::puts("-------------------");
}