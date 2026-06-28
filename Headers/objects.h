//
// Created by EyesightsX on 2026-06-06.
//

#ifndef MAZEGAME_OBJECTS_H
#define MAZEGAME_OBJECTS_H

#include "shader.h"
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_transform.hpp>

struct Wall {

    // Collisions
    float minX, maxX;
    float minZ, maxZ;

    // World Pos
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 rotationAxis;
    float rotationAngle;

    [[nodiscard]] glm::mat4 getModel() const {
        auto model = glm::translate(glm::mat4(1.0f), position);

        if (rotationAxis != glm::vec3(0.0f, 0.0f, 0.0f)) {
            model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
        }

        model = glm::scale(model, size);

        return model;
    }
};

inline float dotPosHalfInterval = 0;
inline float planeSize = 200.0f;

inline bool playerTouchingWall = false;

//TODO ADD Some Thickness
constexpr float WALL_HALF_THICKNESS = 0.05f;

inline std::vector<glm::vec3> worldPosDots;
inline std::vector<Wall> currentWallVector;


class Camera;

unsigned int loadTexture(char const* path);
void setIcon(GLFWwindow* window);

unsigned int registerPlane();  // returns VAO so it can be reused
void drawPlane(unsigned int planeVAO, Shader& planeShader, Camera& fpsCamera, unsigned int planeTexture);

unsigned int registerWall();
void drawWall(unsigned int wallVAO, Shader& wallShader, Camera& fpsCamera, const glm::mat4& model, unsigned int wallTexture);

unsigned int registerCube();
void drawCube(unsigned int cubeVAO, Shader& cubeShader, Camera& fpsCamera, glm::vec3 worldPos, glm::vec3 sizeVec);

std::vector<glm::vec3> getDotsWorldPosVector(int dotNumberOneSided, float planeWidthorLength, glm::vec3 planeBottomRightPoint);

bool isCameraCollided(glm::vec3 playerPos);

#endif //MAZEGAME_OBJECTS_H