#include <memory>
#include <chrono>
#include <iostream>
#include <optional>

template <typename T>
concept TimePoint = requires(T t)
{
    { std::chrono::system_clock::now() } -> std::convertible_to<T>;
};

template <TimePoint T>
class Data final : public std::enable_shared_from_this<Data<T>>
{
public:
    explicit Data(T time = std::chrono::system_clock::now()) noexcept
        : _time(time) {}

    std::shared_ptr<Data<T>> operator++() noexcept
    {
        auto ptr = this->shared_from_this();
        ptr->_time = ptr->_time.value() + std::chrono::hours(24);
        return ptr;
    }

    std::shared_ptr<Data<T>> operator++(int) noexcept
    {
        auto ptr = std::make_shared<Data>(*this);
        ++(*this);
        return ptr;
    }
        
    decltype(auto) GetTime() const noexcept
    {
        return _time;
    }

private:
    std::optional<T> _time{};
    static int x;
};

template <TimePoint T>
std::ostream &operator<<(std::ostream &os, const std::shared_ptr<Data<T>> &data)
{
    if (data && data->GetTime().has_value())
    {
        auto time = std::chrono::system_clock::to_time_t(*(data->GetTime()));
        os << std::ctime(&time);
    }
    return os;
}

int main()
{
    auto data = std::make_shared<Data<std::chrono::system_clock::time_point>>();

    // 原来
    std::cout << data << '\n';

    // 后置
    std::cout << "---------\n";
    std::cout << (*data)++;
    std::cout << data << '\n';

    // 前置
    std::cout << "---------\n";
    std::cout << ++(*data);
    std::cout << data << '\n';
}