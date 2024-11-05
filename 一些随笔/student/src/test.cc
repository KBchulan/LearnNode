#include <memory>
#include <fstream>
#include <iostream>

#include "student.pb.h"

int main()
{
    std::shared_ptr<Student> student = std::make_shared<Student>();
    student->set_name("whx");
    student->set_age(20);
    student->set_num(555);
    student->set_sex("man");

    std::cout << student->name() << std::endl;
    std::cout << student->age() << std::endl;
    std::cout << student->num() << std::endl;
    std::cout << student->sex() << std::endl;
    
    // 序列化
    std::ofstream output("student.txt", std::ios::binary);
    student->SerializeToOstream(&output);
    output.close();

    // 反序列化
    std::shared_ptr<Student> deserialized_student = std::make_shared<Student>();
    std::ifstream input("student.txt", std::ios::binary);
    deserialized_student->ParseFromIstream(&input);
    input.close();
    
    std::cout << deserialized_student->name() << std::endl;
    std::cout << deserialized_student->age() << std::endl;
    std::cout << deserialized_student->num() << std::endl;
    std::cout << deserialized_student->sex() << std::endl;

    return 0;

}