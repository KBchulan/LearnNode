//
// Created by whx on 24-10-22.
//

#include "../include/c++17.hpp"

int main(int argc, char **argv) {
    std::ios::sync_with_stdio(NULL);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::cout << "-------+++-------" << std::endl;
    {
        using namespace CXX_17;
        // fold_workspace();
        // class_auto_judge();
        // class_result_auto();
        // constexpr_lambda_workspace();
        // workspace_10();
        // variant_func();
        optional_func();
    }
    std::cout << "-------+++-------" << std::endl;


    return 0;
}