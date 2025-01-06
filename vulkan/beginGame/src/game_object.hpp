#pragma once
#include <glm/glm.hpp>

class GameObject {
public:
    GameObject(glm::vec2 pos = glm::vec2(0.0f), glm::vec2 size = glm::vec2(1.0f))
        : position(pos), size(size) {}

    virtual void update(float deltaTime) = 0;
    
    glm::vec2 position;
    glm::vec2 size;
    bool isActive = true;
};

class Player : public GameObject {
public:
    Player(glm::vec2 pos = glm::vec2(0.0f))
        : GameObject(pos, glm::vec2(0.1f, 0.1f)), velocity(0.0f) {}

    void update(float deltaTime) override {
        position += velocity * deltaTime;
    }

    void jump() {
        if (isGrounded) {
            velocity.y = jumpForce;
            isGrounded = false;
        }
    }

    glm::vec2 velocity;
    float jumpForce = 5.0f;
    bool isGrounded = true;
};

class Platform : public GameObject {
public:
    Platform(glm::vec2 pos, glm::vec2 size)
        : GameObject(pos, size) {}

    void update(float deltaTime) override {
        // 平台是静态的，不需要更新
    }
}; 