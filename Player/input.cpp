//
// Created by EyesightsX on 2026-06-23.
//

#include "../Headers/input.h"

#include <iostream>

void togglePathCubes(GLFWwindow *window) {
    bool spacePressedNow = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;

    if (spacePressedNow && !spaceKeyWasPressed) {

        cubePathShown = !cubePathShown;
        std::cout << "Switch Mode is " << cubePathShown << std::endl;
    }

    spaceKeyWasPressed = spacePressedNow;
}
