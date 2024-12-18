#include <string>
#include <vector>
#include <boost/sort/pdqsort/pdqsort.hpp>
#include <boost/sort/spreadsort/spreadsort.hpp>

struct Persion 
{
    std::string name;
    std::string phone;
    
    auto operator<=>(const Persion& other) const {
        return name <=> other.name;
    }
};

int main()
{
    std::vector<Persion> persions;
    boost::sort::spreadsort::string_sort(persions.begin(), persions.end());
}
