#include <bits/stdc++.h>

// 判断当前系统是否为大端系统
bool isBigEndian()
{
    int num = 5;
    char *p = (char *)&num;
    return (*p == 0);
}

int main()
{
    if (isBigEndian())
    {
        std::cout << "This system is big-endian." << std::endl;
    }
    else
    {
        std::cout << "This system is little-endian." << std::endl;
    }
    return 0;
}