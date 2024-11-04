// protobuf是用于序列化和反序列化的一种协议，可以用于网络传输、数据存储等。
// 可以将结构化的数据转换为二进制数据，并在网络上传输，也可以将二进制数据转换为结构化的数据。

#include "msgnode.pb.h"

#include <vector>
#include <memory>
#include <string>
#include <iostream>

int main()
{
    std::shared_ptr<Book> book = std::make_shared<Book>();
    book->set_name("败北女角太多了");
    book->set_pages(1000);
    book->set_price(10);

    std::cout << book->name() << std::endl;
    std::cout << book->pages() << std::endl;
    std::cout << book->price() << std::endl;

    std::string bookstr;
    book->SerializeToString(&bookstr);
    std::cout << bookstr << std::endl;

    std::shared_ptr<Book> newbook = std::make_shared<Book>();
    newbook->ParseFromString(bookstr);

    std::cout << newbook->name() << std::endl;
    std::cout << newbook->pages() << std::endl;
    std::cout << newbook->price() << std::endl;

    return 0;
}
