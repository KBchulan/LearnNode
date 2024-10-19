//
// Created by whx on 24-10-14.
//

#include "../include/chrono.hpp"
#include "../include/thread.hpp"
#include "../include/multhread.hpp"
#include "../include/this_whread.hpp"

int main(int argc,char **argv) {
    std::ios::sync_with_stdio(NULL);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    {
        using namespace Thread;
        // create_workspace();
        // exit_workspace();
        // recycle_workspace();
        // detach_workspace();
        // cancel_workspace();
        // class_workspace();
    }
    {
        using namespace MULTHREAD;
        // count_workspace();
        // restrict_learn();
        // mutex_workspace();
        // rwlock_workspace();
        // condition_callback();
        // sem_workspace();
    }
    {
        using namespace CHRONO;
        // duration_workspace();
        // time_point_workspace();
        // system_clock_workspace();
        // steady_clock_workspace();
        // time_point_cast_workspace();
    }
    std::cout << "------------++++-------------" << std::endl;
    {
        using namespace THISTHREAD;
        get_id_workspace();
    }
    std::cout << "------------++++-------------" << std::endl;


    return 0;
}
