#include "Defer.hpp"

utils::Defer::Defer(std::function<void()> callback)
{
  this->callback_ = callback;
}

utils::Defer::~Defer()
{
  callback_();
}