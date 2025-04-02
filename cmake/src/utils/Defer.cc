#include "Defer.hpp"

#include <iostream>

utils::Defer::Defer(std::function<void()> callback)
{
  this->callback_ = callback;
}

utils::Defer::~Defer()
{
  callback_();
}

void utils::Defer::visiable()
{
  std::puts("visiable");
}

void utils::Defer::unvisiable()
{
  std::puts("unvisiable");
}