#include <concepts>
#include <iostream>
#include <unordered_map>
#include <boost/multiprecision/cpp_int.hpp>

using int_ = boost::multiprecision::int1024_t;

std::unordered_map<int, int_> memo;
int_ fib(int n)
{
    if (n <= 2)
        return 1;
    
    if(memo.find(n) != memo.end())
        return memo[n];

    memo[n] = fib(n - 1) + fib(n - 2);
    return memo[n];
}

int main()
{
    int n;
    while (std::cin >> n)
    {
        std::cout << fib(n) << '\n';
    }
}