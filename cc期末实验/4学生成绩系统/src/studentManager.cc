#include "../include/studentManager.h"

void StudentManager::addGrade(const std::string &name, const std::string &subject, double score) noexcept
{
    _studentGrades[name].emplace_back(subject, score);
}

std::vector<Grade> StudentManager::getStudentGrades(const std::string &name) const noexcept
{
    if(_studentGrades.contains(name))
    {
        return _studentGrades.at(name);
    }
    return {};
}

const std::unordered_map<std::string, std::vector<Grade>> &StudentManager::getAllGrades() const noexcept
{
    return _studentGrades;
}