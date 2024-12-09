#ifndef CONST_H
#define CONST_H

#include <vector>
#include <memory>
#include <string>
#include <numeric>
#include <utility>
#include <concepts>
#include <algorithm>
#include <functional>
#include <unordered_map>

// RAII
template <typename T>
class Defer
{
public:
    explicit Defer(T &&func) : _func(std::forward<T>(func)) {}

    ~Defer(){ _func(); }

private:
    T _func;
};

#endif // !CONST_H