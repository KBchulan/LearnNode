#include "physics_engine.hpp"
#include <cuda_runtime.h>

__global__ void updatePositions(glm::vec2* positions, glm::vec2* velocities, float deltaTime, int numObjects) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < numObjects) {
        positions[idx] += velocities[idx] * deltaTime;
        
        // 简单的重力效果
        velocities[idx].y -= 9.81f * deltaTime;
        
        // 边界碰撞检测
        if (positions[idx].y < -1.0f) {
            positions[idx].y = -1.0f;
            velocities[idx].y = -velocities[idx].y * 0.8f;
        }
    }
}

void PhysicsEngine::init(int numObjects) {
    positions.resize(numObjects);
    velocities.resize(numObjects);
    
    // 初始化位置和速度
    for (int i = 0; i < numObjects; i++) {
        positions[i] = glm::vec2(0.0f);
        velocities[i] = glm::vec2(1.0f, 2.0f);
    }
}

void PhysicsEngine::update(float deltaTime) {
    glm::vec2 *d_positions, *d_velocities;
    
    // 分配GPU内存
    cudaMalloc(&d_positions, positions.size() * sizeof(glm::vec2));
    cudaMalloc(&d_velocities, velocities.size() * sizeof(glm::vec2));
    
    // 复制数据到GPU
    cudaMemcpy(d_positions, positions.data(), positions.size() * sizeof(glm::vec2), cudaMemcpyHostToDevice);
    cudaMemcpy(d_velocities, velocities.data(), velocities.size() * sizeof(glm::vec2), cudaMemcpyHostToDevice);
    
    // 启动CUDA核心
    int blockSize = 256;
    int numBlocks = (positions.size() + blockSize - 1) / blockSize;
    updatePositions<<<numBlocks, blockSize>>>(d_positions, d_velocities, deltaTime, positions.size());
    
    // 复制结果回CPU
    cudaMemcpy(positions.data(), d_positions, positions.size() * sizeof(glm::vec2), cudaMemcpyDeviceToHost);
    cudaMemcpy(velocities.data(), d_velocities, velocities.size() * sizeof(glm::vec2), cudaMemcpyDeviceToHost);
    
    // 释放GPU内存
    cudaFree(d_positions);
    cudaFree(d_velocities);
}

std::vector<glm::vec2> PhysicsEngine::getPositions() const {
    return positions;
} 