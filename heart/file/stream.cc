#include <string>
#include <fstream> // file
#include <sstream> // stream
#include <iostream>

/*
file mode:
ios::in：打开文件进行输入（读取）。
ios::out：打开文件进行输出（写入）。
ios::app：打开文件并追加内容到文件末尾。
ios::trunc：打开文件时清空文件内容（默认行为，除非以 ios::app 打开）。
ios::binary：以二进制模式打开文件。
ios::ate：文件打开后指针直接指向文件末尾。
*/

/*
stream status:
is_open()：检查文件是否成功打开。
eof()：检查文件是否读取到末尾。
fail()：检查文件操作是否失败。
*/

std::ostream &operator<<(std::ostream &os, std::ifstream &file)
{
    std::string line;
    while (getline(file, line))
    {
        std::cout << line << '\n';
    }
    return os;
}

decltype(auto) file()
{
    // file
    auto outStream = [str = "write text"]()
    {
        std::ofstream outfile("./write.txt", std::ios::out | std::ios::app);

        if (!outfile.is_open())
        {
            std::cout << "the file open failed" << std::endl;
            return -1;
        }

        outfile << str << '\n';
        outfile.flush();

        outfile.close();
        return 0;
    };

    auto inStream = []()
    {
        std::ifstream infile("./read.txt", std::ios::in);

        if (!infile.is_open())
        {
            std::cout << "the file open failed" << std::endl;
            return -1;
        }

        std::cout << infile;
        infile.close();

        return 0;
    };

    outStream();
    inStream();
    return 0;
}

// 字符串流主要是将数据与字符串进行互化
decltype(auto) stream()
{
    auto instringstream = []()
    {
        std::string str = "100 3.24 hello";

        std::istringstream instream(str, std::ios::in);
        int num;
        double f;
        std::string word;

        instream >> num >> f >> word;
        std::cout << num << " " << f << " " << word << std::endl;
    };

    auto outstringstream = []()
    {
        std::ostringstream outstream;
        int num = 5;
        std::string name = "sam";

        outstream << num << " " << name;

        std::ifstream infile("./read.txt", std::ios::in);

        outstream << infile.rdbuf();

        std::cout << outstream.str() << std::endl;
    };

    instringstream();
    outstringstream();
}

int main()
{
    // file();
    // stream();
}

/*
cin：
cin.get()：从输入流中读取单个字符。
cin.ignore()：忽略输入流中的一定数量的字符（用于跳过某些输入）。
cin.peek()：查看输入流中的下一个字符，但不移除它。
cin >> variable：提取流中的数据到指定变量。

cout：
cout.put()：输出一个字符。
cout.flush()：刷新输出缓冲区，强制将数据输出。
cout.precision()：设置浮点数输出的精度。
*/