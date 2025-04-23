/*
  该文件仅仅是线程池的测试，不关乎实现
*/

#include "ThreadPool.hpp"
#include <chrono>
#include <iostream>
#include <mutex>
#include <numeric>
#include <random>
#include <vector>

void test_thread_pool() {
  std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency() << '\n';
  std::cout << "Available threads: " << threadPool.availableThread() << '\n';

  std::mutex cout_mutex;

  {
    std::cout << "=== Basic functionality test ===" << '\n';

    threadPool.commit([&cout_mutex]() {
      std::lock_guard<std::mutex> lock(cout_mutex);
      std::cout << "Task 1: Completed, Thread ID: " << std::this_thread::get_id() << '\n';
    });

    auto future1 = threadPool.commit([]() -> int {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      return 42;
    });

    auto future2 = threadPool.commit(
        [](int num1, int num2) {
          std::this_thread::sleep_for(std::chrono::milliseconds(200));
          return num1 + num2;
        },
        10, 20);

    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << "Task 2 result: " << future1.get() << '\n';
    std::cout << "Task 3 result: " << future2.get() << '\n';
  }

  {
    std::cout << "\n=== Performance test ===" << '\n';

    const int NUM_TASKS = 10000;
    std::vector<std::future<int>> results;
    results.reserve(NUM_TASKS);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_TASKS; ++i) {
      results.emplace_back(threadPool.commit([i]() {
        int sum = 0;
        for (int j = 0; j < 1000; ++j) {
          sum += (i * j) % 10;
        }
        return sum;
      }));
    }

    int total = 0;
    for (auto &future : results) {
      total += future.get();
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    std::cout << "Execution of " << NUM_TASKS << " tasks took: " << duration.count() << "ms" << '\n';
    std::cout << "Total sum: " << total << '\n';
    std::cout << "Average task duration: " << static_cast<double>(duration.count()) / NUM_TASKS << "ms" << '\n';
  }

  {
    std::cout << "\n=== Exception handling test ===" << '\n';

    auto future = threadPool.commit([]() -> int {
      throw std::runtime_error("Test exception");
      return 0;
    });

    try {
      future.get();
    } catch (const std::exception &e) {
      std::cout << "Successfully caught exception: " << e.what() << '\n';
    }

    auto check_future = threadPool.commit([]() { return "Thread pool is still working"; });

    std::cout << "Thread pool status after exception: " << check_future.get() << '\n';
  }

  {
    std::cout << "\n=== Concurrent scenario simulation ===" << '\n';

    std::vector<int> shared_data(1000, 0);
    std::atomic<int> sum{0};

    std::vector<std::future<void>> tasks;
    tasks.reserve(150);

    for (int i = 0; i < 100; ++i) {
      tasks.emplace_back(threadPool.commit([&shared_data, i]() {
        std::random_device rand;
        std::mt19937 gen(rand());
        std::uniform_int_distribution<> dis(0, 999);

        for (int j = 0; j < 100; ++j) {
          auto idx = static_cast<unsigned long long>(dis(gen));
          shared_data[idx] += (i + 1);
          std::this_thread::sleep_for(std::chrono::microseconds(50));
        }
      }));
    }

    for (int i = 0; i < 50; ++i) {
      tasks.emplace_back(threadPool.commit([&shared_data, &sum]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        int local_sum = 0;
        for (const auto &val : shared_data) {
          local_sum += val;
        }

        sum.fetch_add(local_sum, std::memory_order_relaxed);
      }));
    }

    for (auto &func : tasks) {
      func.wait();
    }

    int actual_sum = std::accumulate(shared_data.begin(), shared_data.end(), 0);

    std::cout << "Final data sum: " << actual_sum << '\n';
    std::cout << "Read task calculation sum: " << sum.load() / 50 << '\n';
  }

  std::cout << "\nAll tests completed, thread pool is running normally" << '\n';
}
