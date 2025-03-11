#ifndef CONST_H
#define CONST_H

// person
#include "Singleton.hpp"

// cc
#include <queue>
#include <tuple>
#include <vector>
#include <thread>
#include <functional>
#include <condition_variable>

// third party
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class RAII
{
public:
    RAII(std::function<void()> func) : _func(func) {}

    ~RAII() { _func(); }

private:
    std::function<void()> _func;
};

// template <typename ReturnType, typename... Args>
// class RAII
// {
// public:
//     explicit RAII(std::function<ReturnType(Args...)> func) : _func(func) {}
//     ~RAII() { std::apply(_func, _args); }

//     void SetArgs(Args... args)
//     {
//         _args = std::make_tuple(std::forward<Args>(args)...);
//     }

// private:
//     std::tuple<Args...> _args;
//     std::function<ReturnType(Args...)> _func;
// };

#endif // !CONST_H