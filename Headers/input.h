//
// Created by EyesightsX on 2026-06-23.
//

#ifndef INPUT_H
#define INPUT_H
#include <GLFW/glfw3.h>

inline bool cubePathShown = false;
inline bool cursorShown = false;

inline bool spaceKeyWasPressed = false;
inline bool ctrlWasPressedNow = false;

void togglePathCubes(GLFWwindow *window);
void toggleCursor(GLFWwindow *window);

#endif //MAZEGAME_INPUT_H