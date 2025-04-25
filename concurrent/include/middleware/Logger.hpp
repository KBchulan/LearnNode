/******************************************************************************
 *
 * @file       Logger.hpp
 * @brief      全局的日志组件
 *
 * @author     KBchulan
 * @date       2025/04/03
 * @history
 ******************************************************************************/

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ranges.h>

#include <cstdint>
#include <global/Singleton.hpp>
#include <string_view>

namespace middleware {

enum class LogLevel : std::uint8_t {
  TRACE,
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  FATAL
};

class Logger final : public global::Singleton<Logger> {
 public:
  // 标准打印
  template <typename... Args>
  void print(std::string_view format, Args &&...args) const noexcept {
    fmt::print(fmt::runtime(format), std::forward<Args>(args)...);
    fmt::print("\n");
  }

  // 彩色打印
  template <typename... Args>
  void print(const fmt::text_style &style, std::string_view format, Args &&...args) const noexcept {
    fmt::print(style, fmt::runtime(format), std::forward<Args>(args)...);
    fmt::print("\n");
  }

  // 日志级别打印
  template <typename... Args>
  void log(LogLevel level, const fmt::text_style &style, std::string_view format, Args &&...args) const noexcept {
    fmt::print(style, "[{}] ", getLevelString(level));
    fmt::print(style, fmt::runtime(format), std::forward<Args>(args)...);
    fmt::print("\n");
  }

  template <typename... Args>
  void trace(std::string_view format, Args &&...args) const noexcept {
    log(LogLevel::TRACE, fmt::fg(fmt::color::gray), format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void debug(std::string_view format, Args &&...args) const noexcept {
    log(LogLevel::DEBUG, fmt::fg(fmt::color::blue), format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void info(std::string_view format, Args &&...args) const noexcept {
    log(LogLevel::INFO, fmt::fg(fmt::color::green), format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void warning(std::string_view format, Args &&...args) const noexcept {
    log(LogLevel::WARNING, fmt::fg(fmt::color::yellow), format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void error(std::string_view format, Args &&...args) const noexcept {
    log(LogLevel::ERROR, fmt::fg(fmt::color::red), format, std::forward<Args>(args)...);
  }

  template <typename... Args>
  void fatal(std::string_view format, Args &&...args) const noexcept {
    log(LogLevel::FATAL, fmt::fg(fmt::color::red), format, std::forward<Args>(args)...);
  }

  // 展示所有的效果示例
  void showExample() const noexcept {
    print("This is a normal message");
    print(fmt::fg(fmt::color::red), "This is a red message");
    trace("This is a trace message");
    debug("This is a debug message");
    info("This is an info message");
    warning("This is a warning message");
    error("This is an error message");
    fatal("This is a fatal message");
  }

 private:
  static std::string_view getLevelString(LogLevel level) noexcept {
    switch (level) {
      case LogLevel::TRACE:   return "TRACE";
      case LogLevel::DEBUG:   return "DEBUG";
      case LogLevel::INFO:    return "INFO";
      case LogLevel::WARNING: return "WARNING";
      case LogLevel::ERROR:   return "ERROR";
      case LogLevel::FATAL:   return "FATAL";
      default:                return "UNKNOWN";
    }
  }
};

}  // namespace middleware

#define logger middleware::Logger::getInstance()

#endif  // LOGGER_HPP
