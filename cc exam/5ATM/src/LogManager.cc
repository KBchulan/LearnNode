#include "../include/LogManager.h"

LogManager::~LogManager()
{
    if(!_is_shutdown)
    {
        Shutdown();
    }
    spdlog::shutdown();
    std::cerr << "LogManager has been destructed\n";
}

bool LogManager::Init(const std::string &log_path, LogLevel level)
{
    try
    {
        _log_path = log_path;

        if(!boost::filesystem::exists(_log_path))
        {
            boost::filesystem::create_directories(_log_path);
        }

        // one thread
        spdlog::init_thread_pool(8192, 1);

        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%t] %v");

        _sql_file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(_log_path + "/sql.log", 0, 0);
        _server_file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(_log_path + "/server.log", 0, 0);

        _sql_logger = CreateLogger("sql", "sql.log");
        _server_logger = CreateLogger("server", "server.log");

        if(!_server_logger || !_sql_logger)
            return false;

        SetLevel(level);
        LOG_SERVER->info("LogManager initialized successfully");
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << "LogManager init failed: " << e.what() << std::endl;
        return false;
    }
}

std::shared_ptr<spdlog::logger> LogManager::CreateLogger(const std::string &logger_name, const std::string &file_name)
{
    try
    {
        auto console_sink =std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(_log_path + "/" + file_name, 0, 0);

        std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};

        auto logger = std::make_shared<spdlog::async_logger>(
            logger_name,
            sinks.begin(),
            sinks.end(),
            spdlog::thread_pool(),
            spdlog::async_overflow_policy::block
        );

        spdlog::register_logger(logger);

        return logger;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Create logger failed: " << e.what() << std::endl;
        return nullptr;
    }
    
}

void LogManager::SetLevel(LogLevel level)
{
    spdlog::set_level(static_cast<spdlog::level::level_enum>(level));
}

void LogManager::Flush()
{
    spdlog::apply_all([](std::shared_ptr<spdlog::logger> logger) -> void
    {
        logger->flush();
    });
}

bool LogManager::GetIsShutdown() const
{
    return _is_shutdown;
}

std::shared_ptr<spdlog::logger> LogManager::GetServerLogger()
{
    return _server_logger;
}

std::shared_ptr<spdlog::logger> LogManager::GetSqlLogger()
{
    return _sql_logger;
}

void LogManager::Shutdown()
{
    if (!_is_shutdown.exchange(true))
    {
        try
        {
            // 刷新所有日志
            Flush();
            
            // 重置 logger
            if (_sql_logger)
            {
                spdlog::drop(_sql_logger->name());
                _sql_logger.reset();
            }
            
            if (_server_logger)
            {
                spdlog::drop(_server_logger->name());
                _server_logger.reset();
            }
            
            // 重置 sink
            _sql_file_sink.reset();
            _server_file_sink.reset();
        }
        catch(const std::exception& e)
        {
            std::cerr << "LogManager shutdown error: " << e.what() << '\n';
        }
    }
}
