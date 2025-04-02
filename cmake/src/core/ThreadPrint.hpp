#ifndef CORE_THREADPRINT_HPP
#define CORE_THREADPRINT_HPP

#include <span>
#include <mutex>
#include <vector>
#include <thread>

#include "core_export.hpp"

namespace core
{
  class CORE_EXPORT ThreadPrint
  {
  public:
    explicit ThreadPrint();

    void print();

  private:
    ThreadPrint(const ThreadPrint &) = delete;
    ThreadPrint(const ThreadPrint &&) = delete;
    ThreadPrint &operator=(ThreadPrint &) = delete;
    ThreadPrint &operator=(ThreadPrint &&) = delete;
  };
}

#endif // !CORE_THREADPRINT_HPP