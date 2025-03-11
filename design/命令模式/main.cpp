//
// Created by whx on 24-10-31.
//

/**
命令模式就是将请求转换为一个包含与请求相关的所有信息的独立对象，通过这个转换能够让使用者根据不同的请求将客户参数化、
延迟请求执行或将请求放入队列中或记录请求日志， 且能实现可撤销操作。
**/

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <unordered_map>

class Cooker {
public:
    void makeDSX() const // NOLINT(*-convert-member-functions-to-static)
    {
        std::cout << "开始烹饪地三鲜";
    }
    void makeGBJD() const // NOLINT(*-convert-member-functions-to-static)
    {
        std::cout << "开始烹饪宫保鸡丁";
    }
    void makeYXRS() const // NOLINT(*-convert-member-functions-to-static)
    {
        std::cout << "开始烹饪鱼香肉丝";
    }
    void makeHSPG() const // NOLINT(*-convert-member-functions-to-static)
    {
        std::cout << "开始烹饪红烧排骨";
    }
};

class AbstractCommand {
public:
    explicit AbstractCommand(const std::shared_ptr<Cooker>& cooker) : _cooker(cooker){}
    virtual ~AbstractCommand() = default;

    virtual void excute() = 0;
    virtual std::string name() = 0;

protected:
    std::shared_ptr<Cooker> _cooker;
};

// 地三鲜的命令
class DSXCommand final : public AbstractCommand
{
public:
    using AbstractCommand::AbstractCommand;

    void excute() override
    {
        _cooker->makeDSX();
    }

    std::string name() override
    {
        return "地三鲜";
    }
};

// 宫保鸡丁的命令
class GBJDCommand final : public AbstractCommand
{
public:
    using AbstractCommand::AbstractCommand;

    void excute() override
    {
        _cooker->makeGBJD();
    }

    std::string name() override
    {
        return "宫保鸡丁";
    }
};

// 鱼香肉丝的命令
class YXRSCommand final : public AbstractCommand
{
public:
    using AbstractCommand::AbstractCommand;

    void excute() override
    {
        _cooker->makeYXRS();
    }

    std::string name() override
    {
        return "鱼香肉丝";
    }
};

// 红烧排骨的命令
class HSPGCommand final : public AbstractCommand
{
public:
    using AbstractCommand::AbstractCommand;

    void excute() override
    {
        _cooker->makeHSPG();
    }

    std::string name() override
    {
        return "红烧排骨";
    }
};

class Waiter {
public:
    void setOrder(const int idx, const std::shared_ptr<AbstractCommand>& cmd) { // NOLINT(*-convert-member-functions-to-static)
        std::cout << idx << "号桌点了" << cmd->name() << '\n';
        if(cmd->name() == "红烧排骨") {
            std::cout << "没有排骨了" << '\n';
            return;
        }

        if(!_cmd_list.contains(idx))
            _cmd_list[idx] = std::vector{cmd};
        else
            _cmd_list[idx].emplace_back(cmd);
    }

    void cancelOrder(const int idx, const std::shared_ptr<AbstractCommand>& cmd) {
        if(_cmd_list.contains(idx)) {
            auto& command = _cmd_list[idx];
            std::erase_if(command,
                [&cmd](const std::shared_ptr<AbstractCommand>& c)
                    {
                              return c == cmd;
                    });
        }
    }

    void showAll(const int idx) {
        for(const auto& it : _cmd_list[idx]) {
            std::cout << it->name() << " ";
        }
        std::cout << '\n';
    }

    void make(const int idx) {
        for(const auto& it : _cmd_list[idx]) {
            it->excute();
        }
    }

private:
    std::unordered_map<int, std::vector<std::shared_ptr<AbstractCommand>>> _cmd_list;
};

int main() {
    std::shared_ptr<Cooker> cooker(new Cooker());
    std::shared_ptr<Waiter> waiter(new Waiter());

    const std::shared_ptr<AbstractCommand> yxrs = std::make_shared<YXRSCommand>(cooker);
    const std::shared_ptr<AbstractCommand> gbjd = std::make_shared<GBJDCommand>(cooker);
    const std::shared_ptr<AbstractCommand> dsx = std::make_shared<DSXCommand>(cooker);
    const std::shared_ptr<AbstractCommand> hspg = std::make_shared<HSPGCommand>(cooker);

    waiter->setOrder(1, yxrs);
    waiter->setOrder(1, dsx);
    waiter->setOrder(1, gbjd);
    waiter->setOrder(1, hspg);
    waiter->setOrder(2, yxrs);

    waiter->cancelOrder(1, yxrs);

    waiter->showAll(1);
    waiter->showAll(2);

    waiter->make(1);
    waiter->make(2);
}