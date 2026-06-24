//
// Created by EyesightsX on 2026-06-23.
//

#ifndef DEBUG_H
#define DEBUG_H

#include "camera.h"

inline bool showWireframe = false;

void interfaceInit(GLFWwindow *window);
void showDebugInterface(bool& showDevUI, Camera &playerCamera);

#endif //MAZEGAME_DEBUG_H