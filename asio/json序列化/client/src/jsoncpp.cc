// #include <fstream>
// #include <iostream>
// #include <jsoncpp/json/json.h>
// #include <jsoncpp/json/value.h>
// #include <jsoncpp/json/reader.h>

// int main()
// {
//     std::ofstream outfile("../text/text.json", std::ios::out);

//     if (!outfile.is_open())
//     {
//         std::cout << R"(文件打开失败)" << std::endl;
//         return -1;
//     }

//     Json::Value root_01;
//     root_01["id"] = 100;
//     root_01["name"] = "whx";

//     std::string strs = root_01.toStyledString();
//     outfile << strs;

//     Json::Value root_02;
//     Json::Reader reader;
//     reader.parse(strs, root_02);

//     std::cout << root_02["id"] << " " << root_02["name"] << '\n';

//     outfile.flush();
//     outfile.close();
//     return 0;
// }