#include "game_manager.hpp"
#include <GLFW/glfw3.h>

void GameManager::init() {
    physicsEngine.init(100); // 初始化物理引擎，支持100个物体
    createLevel();
}

void GameManager::createLevel() {
    // 创建玩家
    auto playerObj = std::make_unique<Player>(glm::vec2(0.0f, 0.0f));
    player = playerObj.get();
    gameObjects.push_back(std::move(playerObj));

    // 创建平台
    gameObjects.push_back(std::make_unique<Platform>(
        glm::vec2(0.0f, -0.8f),
        glm::vec2(2.0f, 0.2f)
    ));

    // 添加一些障碍物
    gameObjects.push_back(std::make_unique<Platform>(
        glm::vec2(1.0f, -0.4f),
        glm::vec2(0.5f, 0.1f)
    ));
}

void GameManager::update(float deltaTime) {
    // 更新物理
    physicsEngine.update(deltaTime);

    // 更新所有游戏对象
    for (auto& obj : gameObjects) {
        if (obj->isActive) {
            obj->update(deltaTime);
        }
    }

    checkCollisions();
}

void GameManager::handleInput(GLFWwindow* window) {
    if (!player) return;

    // 水平移动
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        player->velocity.x = -2.0f;
    } else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        player->velocity.x = 2.0f;
    } else {
        player->velocity.x = 0.0f;
    }

    // 跳跃
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        player->jump();
    }
}

void GameManager::checkCollisions() {
    if (!player) return;

    for (const auto& obj : gameObjects) {
        if (!obj->isActive || obj.get() == player) continue;

        // 简单的AABB碰撞检测
        bool collisionX = player->position.x + player->size.x >= obj->position.x &&
                         obj->position.x + obj->size.x >= player->position.x;
        bool collisionY = player->position.y + player->size.y >= obj->position.y &&
                         obj->position.y + obj->size.y >= player->position.y;

        if (collisionX && collisionY) {
            // 处理碰撞
            if (player->velocity.y < 0) {
                player->position.y = obj->position.y + obj->size.y;
                player->velocity.y = 0;
                player->isGrounded = true;
            }
        }
    }
}

GameManager::~GameManager() {
    gameObjects.clear();
    player = nullptr;
    delete player;
}
