#include "Memory.hpp"

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstddef>
#include <iomanip>
#include <sstream>
#include <middleware/Logger.hpp>

namespace core {

/*
  brk: Linux内核提供用于调整进程的堆内存大小
    - 调用：int brk(void *addr);
    - 介绍：设置进程的program break地址为传入的addr，即堆内存的上边界，用来扩展和收缩我们的堆内存 
  sbrk: 增加堆内存的函数
    - 调用：void *sbrk(intptr_t increment);
    - 介绍：调用sbrk会增加 increment个字节的堆内存，因此调用sbrk(0)可以用来获取当前堆内存的上边界
*/
void Memory::brkCall() noexcept {
  void *initial_break = sbrk(0);
  logger.info("initial program break position: {}", ptrToString(initial_break));

  int *array = (int *)sbrk(10 * sizeof(int));
  logger.info("the return position: {}", ptrToString(array));

  void *after_break = sbrk(0);
  logger.info("after program break position: {}", ptrToString(after_break));

  logger.info("the add size is: {}", (reinterpret_cast<uintptr_t>(after_break) - reinterpret_cast<uintptr_t>(initial_break)));

  for (int i = 0; i < 10; i++) {
    array[i] = i * 100;
    logger.info("array[{}] is: {}", i, array[i]);
  }

  brk(initial_break);
  logger.info("after realease, the program break porstion: {}", ptrToString(sbrk(0)));
}

/*
  mmap: Linux提供的将文件映射在内存中，使得对内存区域的访问会被转化为对文件的访问
   - 调用：void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
   - 介绍：
     - addr：映射区域的首地址，建议设置为空给用户进行选择
     - length：映射区域的字节数
     - prot：指定内存区域的访问权限，如PROT_READ，PROT_WRITE, PROT_EXEC
     - flags：映射类型，如MAP_SHARED(共享，修改会被写回文件), MAP_PRIVATE(私有，不影响源文件), MAP_ANONYMOUS(不映射文件，仅分配内存)
     - fd：文件描述符，指定要映射的文件
     - offset：文件偏移量，指定从文件的某个特定部分开始映射(为4的整数倍)
     - return：成功返回映射区域的起始地址，否则为MAP_FAILED
  munmap：不需要映射时用于解除映射
   - 调用：int munmap(void *addr, size_t length);
   - 介绍：可以传入mmap返回值用于解除映射
*/
void CORE_EXPORT Memory::mmapCall() noexcept {
  const char *path = "/home/whx/桌面/cppreference/linux/src/core/Memory/example.txt";

  auto readExample = [&]() -> void {
    int file_descriptor = open(path, O_RDONLY);
    struct stat file_stat;

    fstat(file_descriptor, &file_stat);
    void *mapped = mmap(nullptr, static_cast<std::size_t>(file_stat.st_size),
                        PROT_READ, MAP_PRIVATE, file_descriptor, 0);
    close(file_descriptor);

    logger.info("size is: {}, file has: {}", file_stat.st_size, reinterpret_cast<char *>(mapped));
    munmap(mapped, static_cast<std::size_t>(file_stat.st_size));
  };

  auto writeExample = [&]() -> void {
    int file_descriptor = open(path, O_RDWR);
    struct stat file_stat;

    fstat(file_descriptor, &file_stat);
    void *mapped = mmap(nullptr, static_cast<std::size_t>(file_stat.st_size),
             PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);
    close(file_descriptor);

    for (int i = 0; i < file_stat.st_size; i++) {
      if (reinterpret_cast<char *>(mapped)[i] >= 'a' && reinterpret_cast<char *>(mapped)[i] <= 'z') {
        reinterpret_cast<char *>(mapped)[i] -= ('a' - 'A');
      }
    }

    msync(mapped, static_cast<std::size_t>(file_stat.st_size), MS_SYNC);
    logger.info("size is: {}, file has: {}", file_stat.st_size, reinterpret_cast<char *>(mapped));
    munmap(mapped, static_cast<std::size_t>(file_stat.st_size));
  };

  readExample();
  writeExample();
}

std::string Memory::ptrToString(void *ptr) noexcept {
  std::stringstream sstream;
  const int width = 12;
  sstream << "0x" << std::hex << std::setw(width) << std::setfill('0') << reinterpret_cast<uintptr_t>(ptr);
  return sstream.str();
}

}  // namespace core
