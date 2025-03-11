#include <vector>
#include <iostream>

int main()
{
    std::vector<int> nums = {1, 2, 3, 4, 5};
    nums.erase(std::remove_if(nums.begin(), nums.end(),
            [](int x)
            {
                return x % 2 == 0;
            }),
        nums.end());

    for (const auto &num : nums)
        std::cout << num << " ";
    std::cout << '\n';
}