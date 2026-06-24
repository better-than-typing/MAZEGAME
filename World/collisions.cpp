//
// Created by EyesightsX on 2026-06-09.
//

#include "../Headers/objects.h"
#include <vector>

#include "../Headers/camera.h"

std::vector<Wall> collisionWalls;

void createWallCollision(float minX, float maxX, float minZ, float maxZ) {
    Wall tempWall = {minX, maxX, minZ, maxZ};
    collisionWalls.push_back(tempWall);
}

bool isCameraCollided(glm::vec3 playerPos) {
    if (collisionWalls.empty()) {
        std::cout << "No Wall Collisions Added!\n";
        return false;
    }

    for (Wall wall : collisionWalls) {
        float closestX = glm::clamp(playerPos.x, wall.minX, wall.maxX);
        float closestZ = glm::clamp(playerPos.z, wall.minZ, wall.maxZ);

        float dx = playerPos.x - closestX;
        float dz = playerPos.z - closestZ;

        if (dx * dx + dz * dz < PLAYER_RADIUS * PLAYER_RADIUS) {
            std::cout << "Collision from: " << playerPos.x << ", " << playerPos.z << std::endl;
            return true;
        }
    }

    return false;
}