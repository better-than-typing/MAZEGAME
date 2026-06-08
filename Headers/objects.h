//
// Created by EyesightsX on 2026-06-06.
//

#ifndef MAZEGAME_OBJECTS_H
#define MAZEGAME_OBJECTS_H

#include "Headers/shader.h"
#include "Headers/camera.h"

unsigned int registerPlane();  // returns VAO so it can be reused
void drawPlane(unsigned int planeVAO, Shader& planeShader, Camera& fpsCamera);

#endif //MAZEGAME_OBJECTS_H