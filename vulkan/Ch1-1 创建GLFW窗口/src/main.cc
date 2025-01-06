#include "../include/GlfwGeneral.hpp"

int main()
{
    if (!InitializeWindow())
        return EXIT_FAILURE;

    while (!glfwWindowShouldClose(Window))
    {
        // TODO: 渲染逻辑
        glfwPollEvents();
        TitleFPS();
    }
    TerminateWindow();
    return EXIT_SUCCESS;
}