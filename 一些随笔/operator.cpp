#include <iostream>

class Test
{
public:
    operator std::string() const
    {
        return "八奈见真可爱";
    }
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

int main()
{
    std::cout << Test{} << std::endl;
}