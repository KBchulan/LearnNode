//
// Created by whx on 24-10-30.
//

/**
将对象连成一条链，并沿着这条链传递请求，直到链上有一个对象将请求处理掉为止，这种处理数据的模式叫做责任链模式。
使用这种模式有一个好处：处理者可以决定不再沿着链传递请求， 这可高效地取消所有后续处理步骤。
**/

#include <memory>
#include <iostream>

enum class RequestType {
    QingJia,
    ZhangXin,
    CiZhi
};

// 管理者结点的抽象
class AbstractManager {
public:
    void set_next(const std::shared_ptr<AbstractManager> &next) {
        _next = next;
    }

    virtual void handle_request(RequestType type) = 0;
    virtual ~AbstractManager() = default;

protected:
    std::shared_ptr<AbstractManager> _next;
};

class Manager final: public AbstractManager {
public:
    void handle_request(RequestType type) override {
        switch (type) {
            case RequestType::QingJia:
                std::cout << "请假: 同意请假，好好休息~~~" << std::endl;
                    break;
            case RequestType::ZhangXin:
                std::cout << "涨薪: 这个我得请示一下咱们CEO..." << " ====> ";
                _next->handle_request(type);
                break;
            case RequestType::CiZhi:
                std::cout << "辞职: 我给你向上级反应一下..." << " ====> ";
                _next->handle_request(type);
                break;
            default:
                break;
        }
    }
};

class CEO final: public AbstractManager {
public:
    void handle_request(RequestType type) override {
        switch (type) {
            case RequestType::QingJia:
                std::cout << "请假: 同意请假, 下不为例..." << std::endl;
                break;
            case RequestType::ZhangXin:
                std::cout << "涨薪: 你工资不少了, 给你个购物券吧..." << " ====> ";
                break;
            case RequestType::CiZhi:
                std::cout << "辞职: 这个我得问问咱们老板..." << " ====> ";
                _next->handle_request(type);
                break;
            default:
                break;
        }
    }
};

class Boss final: public AbstractManager {
public:
    void handle_request(RequestType type) override {
        switch (type)
        {
            case RequestType::QingJia:
                std::cout << "请假: 只有工作才能实现人生价值，回去好好坚守岗位！！！" << std::endl;
            break;
            case RequestType::ZhangXin:
                std::cout << "涨薪: 钱财乃身外之物, 要视其如粪土!!!" << std::endl;
            break;
            case RequestType::CiZhi:
                std::cout << "辞职: 巴洛克工作社就是你的家, 这次把你留下, 下次别再提了!!!" << std::endl;
            break;
            default:
                break;
        }
    }
};

class LuXuanYu {
public:
    static void request(RequestType type, const std::shared_ptr<AbstractManager>& manager) {
        manager->handle_request(type);
    }
};

int main() {
    std::ios::sync_with_stdio(NULL);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::shared_ptr<AbstractManager> manager = std::make_shared<Manager>();
    std::shared_ptr<AbstractManager> ceo = std::make_shared<CEO>();
    std::shared_ptr<AbstractManager> boss = std::make_shared<Boss>();

    manager->set_next(ceo);
    ceo->set_next(boss);

    LuXuanYu::request(RequestType::QingJia, manager);
    LuXuanYu::request(RequestType::ZhangXin, manager);
    LuXuanYu::request(RequestType::CiZhi, manager);

    LuXuanYu::request(RequestType::QingJia, ceo);
    LuXuanYu::request(RequestType::ZhangXin, ceo);
    LuXuanYu::request(RequestType::CiZhi, ceo);

    LuXuanYu::request(RequestType::QingJia, boss);
    LuXuanYu::request(RequestType::ZhangXin, boss);
    LuXuanYu::request(RequestType::CiZhi, boss);
}