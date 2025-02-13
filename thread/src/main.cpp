//
// Created by whx on 24-10-14.
//

#include "../include/chrono.hpp"
#include "../include/thread.hpp"
#include "../include/condition.hpp"
#include "../include/multhread.hpp"
#include "../include/this_thread.hpp"
#include "../include/async_thread.hpp"

int main(int argc,char **argv) {
    std::ios::sync_with_stdio(NULL);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::cout << "------------++++-------------" << std::endl;
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
        // c11_mutex_workspace();
        // c11_recursive_mutex_workspace();
        // c11_timed_mutex_workspace();
    }
    {
        using namespace CHRONO;
        // duration_workspace();
        // time_point_workspace();
        // system_clock_workspace();
        // steady_clock_workspace();
        // time_point_cast_workspace();
    }
    {
        using namespace THISTHREAD;
        // get_id_workspace();
        // sleep_for_workspace();
        // sleep_until_workspace();
        // yield_workspace();
        // call_once_workspace();
    }
    {
        using namespace CONDITION;
        // condition_workspace();
    }
    {
        using namespace ATOMIC;
        // atomic_workspace();
        // count_with_mutex();
        // count_with_atomic();
    }
    {
        using namespace ASYNCTHREAD;
        // promise_workspace();
        // packaged_task_workspace();
        async_workspace_right_now();
    }
    std::cout << "------------++++-------------" << std::endl;


    return 0;
}
