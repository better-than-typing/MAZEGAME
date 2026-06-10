//
// Created by EyesightsX on 2026-06-06.
//

#ifndef MAZEGAME_OBJECTS_H
#define MAZEGAME_OBJECTS_H

#include "Headers/shader.h"

struct Wall {
    float minX, maxX;
    float minZ, maxZ;
};

class Camera;

unsigned int registerPlane();  // returns VAO so it can be reused
void drawPlane(unsigned int planeVAO, Shader& planeShader, Camera& fpsCamera);

unsigned int registerWall();
void drawWall(unsigned int wallVAO, Shader& wallShader, Camera& fpsCamera, glm::vec3 worldPos);

void createWallCollision(float minX, float maxX, float minZ, float maxZ);
bool isCameraCollided(glm::vec3 playerPos);

#endif //MAZEGAME_OBJECTS_H