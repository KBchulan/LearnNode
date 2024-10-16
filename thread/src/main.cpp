//
// Created by whx on 24-10-14.
//

#include <iostream>

#include "thread.hpp"

int main(int argc,char **argv) {
    std::ios::sync_with_stdio(NULL);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    {
        using namespace Thread;
        // create_workspace();
        // exit_workspace();
        // recycle_workspace();
        detach_workspace();
    }


    return 0;
}
