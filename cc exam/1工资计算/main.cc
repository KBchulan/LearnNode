#include <string>
#include <vector>
#include <memory>
#include <concepts>
#include <fmt/core.h>
#include <boost/multiprecision/cpp_int.hpp>

using int1024_t = boost::multiprecision::int1024_t;

template <typename T>
concept EmployeeConcept = requires(T t) {
    { t.earnings() } -> std::convertible_to<double>;
    { t.get_names() } -> std::convertible_to<std::string>;
};

enum class EmployType
{
    SALARIED,
    COMMISION,
    SALARYCOM
};

class BaseEmployee
{
public:
    explicit BaseEmployee(EmployType type, const std::string &name)
        : _type(type), _name(name), _baseSalary(0), _addSalary(0) {}

    virtual ~BaseEmployee() = default;

    [[nodiscard]] virtual int1024_t GetSalary() = 0;

    [[nodiscard]] const std::string &GetName() const noexcept
    {
        return _name;
    }

protected:
    EmployType _type;
    std::string _name;

    // salary
    int1024_t _addSalary;
    int1024_t _baseSalary;
};

class SalariedEmployee final : public BaseEmployee
{
public:
    explicit SalariedEmployee(const std::string &name, int1024_t baseSalary)
        : BaseEmployee(EmployType::SALARIED, name)
    {
        _baseSalary = baseSalary;
    }

    [[nodiscard]] int1024_t GetSalary() override
    {
        return _baseSalary;
    }
};

class CommisionEmployee final : public BaseEmployee
{
public:
    explicit CommisionEmployee(const std::string &name, int1024_t addSalary)
        : BaseEmployee(EmployType::COMMISION, name)
    {
        _addSalary = addSalary;
    }

    [[nodiscard]] int1024_t GetSalary() override
    {
        return static_cast<int1024_t>(_addSalary);
    }
};

class PlusCommsionEmployee final : public BaseEmployee
{
public:
    explicit PlusCommsionEmployee(const std::string &name, int1024_t baseSalary, int1024_t addSalary)
        : BaseEmployee(EmployType::SALARYCOM, name)
    {
        _baseSalary = baseSalary;
        _addSalary = addSalary;
    }

    [[nodiscard]] int1024_t GetSalary() override
    {
        return _baseSalary + _addSalary + _baseSalary / 10;
    }
};

// employee factory
class EmployeeFactory
{
public:
    static std::unique_ptr<BaseEmployee> CreateEmployee(EmployType type, const std::string &name, int1024_t baseSalary, int1024_t addSalary)
    {
        switch (type)
        {
        case EmployType::SALARIED:
            return std::make_unique<SalariedEmployee>(name, baseSalary);
        case EmployType::COMMISION:
            return std::make_unique<CommisionEmployee>(name, addSalary);
        case EmployType::SALARYCOM:
            return std::make_unique<PlusCommsionEmployee>(name, baseSalary, addSalary);
        default:
            return nullptr;
        }
    }
};

template <> 
struct fmt::formatter<int1024_t> 
{
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) 
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const int1024_t& n, FormatContext& ctx) const -> decltype(ctx.out()) 
    {
        return fmt::format_to(ctx.out(), "{}", n.str());
    }
};

class Company
{
public:
    void AddEmployee(std::unique_ptr<BaseEmployee> employee)
    {
        _employees.push_back(std::move(employee));
    }

    void PrintAll() const
    {
        fmt::print("公司工资表：\n");
        fmt::print("------------------------\n");
        for (const auto &emp : _employees)
        {
            fmt::print("员工：{:<10} 工资：{}\n",
                       emp->GetName(), emp->GetSalary());
        }
        fmt::print("------------------------\n");
    }

private:
    std::vector<std::unique_ptr<BaseEmployee>> _employees;
};

int main()
{
    Company company;

    company.AddEmployee(EmployeeFactory::CreateEmployee(
        EmployType::SALARIED, "张三", 5000, 0));
    
    company.AddEmployee(EmployeeFactory::CreateEmployee(
        EmployType::COMMISION, "李四", 0, 6000));
    
    company.AddEmployee(EmployeeFactory::CreateEmployee(
        EmployType::SALARYCOM, "王五", 3000, 500));
    
    company.PrintAll();
 
}