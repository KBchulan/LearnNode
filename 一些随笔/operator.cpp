#include <iostream>

class Test
{
public:
    operator std::string() const
    {
        return "八奈见真可爱";
    } 

    int num;
};

std::ostream &operator<<(std::ostream &os, const std::string &str)
{
    os << str.c_str();
    return os;
}

std::ostream &operator<<(std::ostream &os, const Test &test)
{
    std::string str = test;
    os << str;
    return os;
}

void operator+=(Test &t1, Test &t2)
{
    t1.num += t2.num;
}

int main()
{
    std::cout << Test{} << std::endl;
    Test *t1 = new Test{1};
    Test *t2 = new Test{2};
    *t1 += *t2;
    std::cout << (*t1).num << std::endl;
    delete t1;
    delete t2;
}