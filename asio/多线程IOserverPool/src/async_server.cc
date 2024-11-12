//
// Created by whx on 24-11-8.
//
// condition_variable to exit this server

// #include "../include/CServer.hpp"

// #include <mutex>
// #include <thread>
// #include <csignal>
// #include <condition_variable>

// std::mutex g_mutex;
// bool is_quit = false;
// std::condition_variable g_cv;

// void singal_handle(int singal)
// {
//     std::unique_lock<std::mutex> locker(g_mutex);
//     is_quit = true;
//     g_cv.notify_one();
// }

// int main(int argc, char **argv)
// {
//     try
//     {
//         boost::asio::io_context ioc;

//         // 注册信号函数
//         signal(SIGINT, singal_handle);

//         // 开辟一个子线程用来跑服务
//         std::thread net_work_thread([&ioc]
//         {
//             CServer server(ioc, 12569);
//             ioc.run(); 
//         });

//         {
//             std::unique_lock<std::mutex> lock(g_mutex);
//             g_cv.wait(lock, []
//                       { return is_quit; });
//         }

//         ioc.stop();
//         net_work_thread.join();

//         std::cerr << "Server stopped gracefully" << std::endl;
//     }
//     catch (const boost::system::system_error &error)
//     {
//         std::cout << R"(Ioc server failed!)" << '\n';
//         std::cout << R"(The error code is: )" << error.code().value() << '\n';
//         std::cout << R"(The error message is: )" << error.code().message() << '\n';
//     }

//     return 0;
// }
