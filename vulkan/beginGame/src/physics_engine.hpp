#pragma once
#include <glm/glm.hpp>
#include <vector>

class PhysicsEngine {
public:
    void init(int numObjects);
    void update(float deltaTime);
    std::vector<glm::vec2> getPositions() const;

private:
    std::vector<glm::vec2> positions;
    std::vector<glm::vec2> velocities;
}; 