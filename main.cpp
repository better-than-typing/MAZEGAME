#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "External Headers/imgui.h"
#include "External Headers/imgui_impl_glfw.h"
#include "External Headers/imgui_impl_opengl3.h"
#include "Headers/shader.h"
#include "Headers/objects.h"
#include "Headers/camera.h"
#include "Headers/mazegen.h"
#include "Headers/input.h"
#include "Headers/debug.h"

constexpr unsigned int SCREEN_WIDTH = 1280;
constexpr unsigned int SCREEN_HEIGHT = 800;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

bool showDevUI = true;

GLFWwindow* window;
Camera fpsCamera(glm::vec3(0.0f, 0.5f, 0.0f));

void initWindow();
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void processInput(GLFWwindow *window);

//TODO Make the GRID SIZE proportional to wall size or length
glm::vec3 wallSize;
glm::vec3 wallPos1;

int main() {

    initWindow();
    interfaceInit(window);

    unsigned int planeVAO = registerPlane();
    unsigned int wallVAO = registerWall();
    unsigned int dotVAO = registerCube();

    unsigned int lightCubeVAO = registerCube();

    unsigned int planeTexture = loadTexture(R"(C:\Users\EyesightsX\CLionProjects\MazeGame\Assets\WoodFloor051_4K-PNG\WoodFloor051_4K-PNG_Color.png)");

    Shader planeShader(R"(C:\Users\EyesightsX\CLionProjects\MazeGame\Shaders\Plane\planeVS.glsl)", R"(C:\Users\EyesightsX\CLionProjects\MazeGame\Shaders\Plane\planeFS.glsl)");
    Shader dotShader(R"(C:\Users\EyesightsX\CLionProjects\MazeGame\Shaders\Dot\dotVS.glsl)", R"(C:\Users\EyesightsX\CLionProjects\MazeGame\Shaders\Dot\dotFS.glsl)");
    Shader lightShader(R"(C:\Users\EyesightsX\CLionProjects\MazeGame\Shaders\Light\lightVS.glsl)", R"(C:\Users\EyesightsX\CLionProjects\MazeGame\Shaders\Light\lightFS.glsl)");

    Maze::generateMaze();
    while (!glfwWindowShouldClose(window)) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        showDebugInterface(showDevUI, fpsCamera, deltaTime);

        planeShader.use();

        // Frame Handling
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);
        togglePathCubes(window);
        toggleCursor(window);

        // Debug
        if (showWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // Background
        glClearColor(0.0f, 0.0f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Texture Objects
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planeTexture);

        // Draw Objects
        drawPlane(planeVAO, planeShader, fpsCamera);

        // Light
        drawCube(lightCubeVAO, lightShader, fpsCamera, glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f));

        if (cubePathShown) {
            for (glm::vec3 pos : worldPosDots) {
                drawCube(dotVAO, lightShader, fpsCamera, glm::vec3(pos.x * 0.1f, 0.125f, pos.z * 0.1f), glm::vec3(0.25f, 0.25f, 0.25f));
            }
        }

        // End GUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Buffer and Input Event
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}

void initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze Game", nullptr, nullptr);

    if (!window) {
        std::cout << "Failed to create window\n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
    }

    glEnable(GL_DEPTH_TEST);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    setIcon(window);
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {

    if (cursorShown) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    fpsCamera.ProcessMouseMovement(xOffset, yOffset);
}

void processInput(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        fpsCamera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        fpsCamera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        fpsCamera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        fpsCamera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        fpsCamera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        fpsCamera.ProcessKeyboard(DOWN, deltaTime);
}
