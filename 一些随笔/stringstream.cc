#include <bits/stdc++.h>

using namespace std;

template <typename T>
constexpr T pi = T(3.141592653);

template <>
constexpr auto pi<const char *> = "pi";

int main()
{
    cout << pi<const char *> << endl;
    cout << std::fixed << std::setprecision(5) << pi<double> << endl;
    
    string line;

    stringstream stream;
    stream << line;

    int num;

    stream >> num;
}