#include <bits/stdc++.h>

template <typename T>
void *any_to_void(const T &t)
    requires std::is_pointer_v<T> ||
             std::is_function_v<T> ||
             std::is_member_function_pointer_v<T> ||
             std::is_array_v<T>
{
    return *(void **)&(const std::decay_t<T> &)t;
}

void func()
{
    std::cout << "1" << std::endl;
}

int main()
{
    void *ptr = any_to_void(func);
    ((void(*)())ptr)();
}
