#include "../include/LogManager.h"
#include "../include/MainWidget.h"
#include <csignal>
#include <QApplication>

static std::atomic<bool> g_running{true};
static QApplication *g_app = nullptr;

void signalHandler(int signum)
{
    std::cerr << "Interrupt signal (" << signum << ") received.\n";

    g_running = false;
    std::quick_exit(0);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    QApplication app(argc, argv);
    g_app = &app;

    if (!LogManager::GetInstance()->Init("../logs", LogLevel::DEBUG))
    {
        std::cerr << "初始化日志系统失败" << std::endl;
        return EXIT_FAILURE;
    }

    LOG_SERVER->info("Server logger init successfully");

    MainWidget w;
    w.show();

    QObject::connect(&app, &QApplication::aboutToQuit, []()
    {
        if (g_running) 
        {
            LogManager::GetInstance()->Shutdown();
            MysqlManager::GetInstance().reset();
        } 
    });

    int ret = app.exec();
    g_app = nullptr;

    if (g_running)
    {
        LogManager::GetInstance()->Shutdown();
        MysqlManager::GetInstance().reset();
    }

    return ret;
}