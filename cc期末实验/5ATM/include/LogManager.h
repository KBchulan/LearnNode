#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include "const.h"

enum class LogLevel
{
    TRACE = SPDLOG_LEVEL_TRACE,
    DEBUG = SPDLOG_LEVEL_DEBUG,
    INFO = SPDLOG_LEVEL_INFO,
    WARN = SPDLOG_LEVEL_WARN,
    ERROR = SPDLOG_LEVEL_ERROR,
    CRITICAL = SPDLOG_LEVEL_CRITICAL,
    OFF = SPDLOG_LEVEL_OFF
};

class LogManager final : public Singleton<LogManager>
{
    friend class Singleton<LogManager>;

public:
    ~LogManager() override;
    
    // 二段式构造
    bool Init(const std::string &log_path, LogLevel level = LogLevel::DEBUG);

    // 关闭日志系统
    void Shutdown();

    // 设置日志级别
    void SetLevel(LogLevel level);

    // 刷新所有日志
    void Flush();

    // get方法
    bool GetIsShutdown() const;

    std::shared_ptr<spdlog::logger> GetSqlLogger();
    std::shared_ptr<spdlog::logger> GetServerLogger();

private:
    LogManager() = default;

    std::shared_ptr<spdlog::logger> CreateLogger(const std::string &logger_name, const std::string &file_name);

private:
    std::string _log_path{};
    std::atomic<bool> _is_shutdown{false};
    std::shared_ptr<spdlog::logger> _sql_logger;
    std::shared_ptr<spdlog::logger> _server_logger;
    std::shared_ptr<spdlog::sinks::daily_file_sink_mt> _sql_file_sink;
    std::shared_ptr<spdlog::sinks::daily_file_sink_mt> _server_file_sink;
};

#define LOG_SERVER                                \
    if (!LogManager::GetInstance()->GetIsShutdown()) \
    LogManager::GetInstance()->GetServerLogger()

#define LOG_SQL                                      \
    if (!LogManager::GetInstance()->GetIsShutdown()) \
    LogManager::GetInstance()->GetSqlLogger()

#endif // !LOGMANAGER_H