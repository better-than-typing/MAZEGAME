//
// Created by EyesightsX on 2026-06-06.
//

#ifndef MAZEGAME_OBJECTS_H
#define MAZEGAME_OBJECTS_H

#include "Headers/shader.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

struct Wall {

    // Collisions
    float minX, maxX;
    float minZ, maxZ;

    // World Pos
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 rotation;

    glm::mat4 getModel() {
        auto model = glm::translate(glm::mat4(1.0f), position);

        if (rotation != glm::vec3(0.0f, 0.0f, 0.0f)) {
            model = glm::rotate(model, glm::radians(90.0f), rotation);
        }

        model = glm::scale(model, size);

        return model;
    }
};

inline float dotPosHalfInterval = 0;

class Camera;

unsigned int loadTexture(char const* path);
void setIcon(GLFWwindow* window);

unsigned int registerPlane();  // returns VAO so it can be reused
void drawPlane(unsigned int planeVAO, Shader& planeShader, Camera& fpsCamera);

unsigned int registerWall();
// void drawWall(unsigned int wallVAO, Shader& wallShader, Camera& fpsCamera);
void drawWall(unsigned int wallVAO, Shader& wallShader, Camera& fpsCamera, const glm::mat4& model);

unsigned int registerDot();
void drawDot(unsigned int dotVAO, Shader& dotShader, Camera& fpsCamera, glm::vec3 worldPos, glm::vec3 sizeVec);

std::vector<glm::vec3> getDotsWorldPosVector(int dotNumberOneSided, float planeWidthorLength, glm::vec3 planeBottomRightPoint);

void createWallCollision(float minX, float maxX, float minZ, float maxZ);
bool isCameraCollided(glm::vec3 playerPos);

#endif //MAZEGAME_OBJECTS_H