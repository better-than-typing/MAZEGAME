//
// Created by EyesightsX on 2026-06-09.
//

#include "Headers/objects.h"
#include <vector>

std::vector<Wall> collisionWalls;

void createWallCollision(float minX, float maxX, float minZ, float maxZ) {
    Wall tempWall = {minX, maxX, minZ, maxZ};
    collisionWalls.push_back(tempWall);
}

bool isCameraCollided(glm::vec3 playerPos) {
    if (collisionWalls.size() <= 0) {
        std::cout << "No Wall Collisions Added!\n";
        return false;
    }

    for (Wall wall : collisionWalls) {
        if (abs(wall.minZ - playerPos.z) <= 0.15f) {
            return true;
        }
    }

    return false;
}