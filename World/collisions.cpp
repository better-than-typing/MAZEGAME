//
// Created by EyesightsX on 2026-06-09.
//

#include <algorithm>
#include <random>

#include "../Headers/objects.h"
#include <vector>

#include "../Headers/camera.h"

bool isCameraCollided(glm::vec3 playerPos) {

    int i = 0;
    for (Wall wall : currentWallVector) {
        if (wall.minX == wall.maxX || wall.minZ == wall.maxZ) {

            if (abs(playerPos.z - wall.position.z) <= PLAYER_RADIUS && playerPos.x < wall.maxX && playerPos.x > wall.minX) {
                playerTouchingWallIndex = i;
                return true;
            }

            if (abs(playerPos.x - wall.position.x) <= PLAYER_RADIUS && playerPos.z < wall.maxZ && playerPos.z > wall.minZ) {
                playerTouchingWallIndex = i;
                return true;
            }
        }
        i++;
    }

    playerTouchingWallIndex = -1;
    return false;
}

bool isPlayerAtExit(int exitWallIndex) {



    if (playerTouchingWallIndex == exitWallIndex) {
        return true;
    }

    return false;
}


int getExitWallIndex() {

    float maxBorderPos = 0;
    for (const auto& borderPos : currentWallVector) {
        if (borderPos.position.x > maxBorderPos) {
            maxBorderPos = borderPos.position.x;
        }
        if (borderPos.position.z > maxBorderPos) {
            maxBorderPos = borderPos.position.z;
        }
    }

    maxBorderPos = std::floor(maxBorderPos);

    std::cout << "Max Border Pos Float (Floored): " << maxBorderPos << std::endl;

    int numBorderWall = 0;
    std::vector<int> borderWallIndicesVector;
    for (const auto& wall : currentWallVector) {
        if (std::floor(abs(wall.position.x)) == maxBorderPos || std::floor(abs(wall.position.z)) == maxBorderPos) {
            borderWallIndicesVector.push_back(numBorderWall);
        }

        numBorderWall++;
    }
    std::sort(borderWallIndicesVector.begin(), borderWallIndicesVector.end());

    std::cout << currentWallVector.size() << " down to " << borderWallIndicesVector.size() << std::endl;


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, static_cast<int>(borderWallIndicesVector.size()));

    int randomIndex = distrib(gen);
    int exitWallIndex = borderWallIndicesVector[randomIndex];

    return exitWallIndex;
}