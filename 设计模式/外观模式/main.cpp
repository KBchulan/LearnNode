//
// Created by whx on 24-10-30.
//

/**
外观模式就是给很多复杂的子系统提供一个简单的上层接口，并在这些接口中包含用户真正关心的功能
跟写脚本一键启动多个命令一样
**/

#include <iostream>

using namespace std;

int main() {
    auto CokeSys = []() {
        cout << "狮吼炮原料<可乐>已经注入完毕..." << endl;
    };

    auto EnergySys = []() {
        cout << "已经将所有的可乐转换为了能量..." << endl;
    };

    auto AimLockSys = []() {
        cout << "已经瞄准并且锁定了目标..." << endl;
    };

    auto Cannon = []() {
        cout << "狮吼炮正在向目标开火..." << endl;
    };

    auto WindCannon = []() {
        cout << "发射风来炮抵消后坐力稳定船身..." << endl;
    };

    // 同一接口
    auto fire = [&]() {
        CokeSys();
        EnergySys();
        AimLockSys();
        Cannon();
        WindCannon();
    };

    fire();
}