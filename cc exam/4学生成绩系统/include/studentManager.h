#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include "const.h"

struct Grade
{
    double _score{};
    std::string _subject{};

    explicit Grade(const std::string &subject, double score) noexcept
        : _subject(subject), _score(score) {}
    
    operator std::string() const
    {
        return _subject + std::to_string(_score);
    }
};

class StudentManager
{
public:
    static std::unique_ptr<StudentManager> &GetInstance() noexcept
    {
        static std::unique_ptr<StudentManager> instance(new StudentManager());
        return instance;
    }

    ~StudentManager() = default;

    void addGrade(const std::string &name, const std::string &subject, double score) noexcept;

    std::vector<Grade> getStudentGrades(const std::string &name) const noexcept;

    const std::unordered_map<std::string, std::vector<Grade>> &getAllGrades() const noexcept;

    template <std::floating_point T = double>
    T calcuteAverage(const std::string &name) const noexcept
    {
        if(_studentGrades.contains(name))
        {
            const auto &grades = _studentGrades.at(name);
            if(grades.empty())
                return T{0};
            
            T sum = std::accumulate(grades.begin(), grades.end(), T{0}, 
            [](T total, const Grade &grade) -> T
            {
                return total + static_cast<T>(grade._score);
            });
            return sum / static_cast<T>(grades.size());
        }
        return T{0};
    }

private:
    StudentManager() = default;
    StudentManager(const StudentManager &) = delete;
    StudentManager &operator=(const StudentManager &) = delete;

private:
    std::unordered_map<std::string, std::vector<Grade>> _studentGrades;
};

#endif // !STUDENT_MANAGER_H