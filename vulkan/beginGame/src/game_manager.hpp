#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "game_object.hpp"
#include "physics_engine.hpp"
#include <vector>
#include <memory>

class GameManager {
public:
    void init();
    void update(float deltaTime);
    void handleInput(GLFWwindow* window);

    Player* getPlayer() { return player; }
    const std::vector<std::unique_ptr<GameObject>>& getGameObjects() const { return gameObjects; }

    ~GameManager();

private:
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    Player* player;
    PhysicsEngine physicsEngine;

    void createLevel();
    void checkCollisions();
};
