//
// Created by EyesightsX on 2026-06-09.
//

#include "../Headers/objects.h"
#include <vector>

#include "../Headers/camera.h"

bool isCameraCollided(glm::vec3 playerPos) {

    for (Wall wall : currentWallVector) {
        if (wall.minX == wall.maxX) {
            if (abs(playerPos.x - wall.position.x) <= PLAYER_RADIUS && playerPos.z < wall.maxZ && playerPos.z > wall.minZ) {
                playerTouchingWall = true;
                return true;
            }
        }

        if (wall.minZ == wall.maxZ) {
            if (abs(playerPos.z - wall.position.z) <= PLAYER_RADIUS && playerPos.x < wall.maxX && playerPos.x > wall.minX) {
                playerTouchingWall = true;
                return true;
            }
        }
    }
    return false;
}