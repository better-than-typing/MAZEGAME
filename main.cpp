#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Headers/shader.h"
#include "Headers/objects.h"
#include "Headers/camera.h"
#include "Headers/mazegen.h"

constexpr unsigned int SCREEN_WIDTH = 1280;
constexpr unsigned int SCREEN_HEIGHT = 800;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

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

    unsigned int planeVAO = registerPlane();
    unsigned int wallVAO = registerWall();

    // Testing
    std::vector<glm::vec3> worldPosDots = getDotsWorldPosVector(Maze::numDotsOnSide, planeSize, glm::vec3(-planeSize / 2.0f, 0.0f, planeSize / 2.0f));
    std::vector<arrowIndex> initMazeIndicesVector = Maze::initMaze();

    std::vector<arrowIndex> currArrowIndices = initMazeIndicesVector;
    for (int i = 0; i < Maze::numDotsOnSide * Maze::numDotsOnSide * 10; i++) {
        origin nextOrigin = Maze::getRandomOrigin();

        currArrowIndices = Maze::shiftedMazeIndices(nextOrigin, currArrowIndices);
    }


    unsigned int dotVAO = registerDot();

    unsigned int planeTexture = loadTexture(R"(C:\Users\EyesightsX\CLionProjects\MazeGame\Assets\planeTexture.png)");

    createWallCollision(-(0.5f * wallSize.x), 0.5f * wallSize.x, wallPos1.z, wallPos1.z);

    Shader planeShader(R"(C:\Users\EyesightsX\CLionProjects\MazeGame\Shaders\Plane\planeVS.glsl)", R"(C:\Users\EyesightsX\CLionProjects\MazeGame\Shaders\Plane\planeFS.glsl)");
    Shader dotShader(R"(C:\Users\EyesightsX\CLionProjects\MazeGame\Shaders\Dot\dotVS.glsl)", R"(C:\Users\EyesightsX\CLionProjects\MazeGame\Shaders\Dot\dotFS.glsl)");


    //std::cout << "Half Interval: " << dotPosHalfInterval << std::endl;
    Maze::markNodes(currArrowIndices);
    std::vector<Wall> wallVector = Maze::generateWalls(currArrowIndices, worldPosDots);

    int lineBreak = 0;
    for (arrowIndex arrow_index : currArrowIndices) {
        std::cout << arrow_index.xI << ", " << arrow_index.zI << " -> ";
        std::cout << arrow_index.xF << ", " << arrow_index.zF << "   ";
        lineBreak++;

        if (lineBreak % Maze::numDotsOnSide == 0) {
            std::cout << std::endl;

            lineBreak = 0;
        }
    }

    for (Wall wall : wallVector) {
        //createWallCollision(-(0.5f * wall.size.x), 0.5f * wall.size.x, 0.0f, 0.0f);
    }

    while (!glfwWindowShouldClose(window)) {

        planeShader.use();

        // Frame Handling
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);

        // Background
        glClearColor(0.0f, 0.0f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Texture Objects
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, planeTexture);

        // Draw Objects
        drawPlane(planeVAO, planeShader, fpsCamera);

        //glm::vec3 testPos = worldPosDots[0];
        //testPos.x = (worldPosDots[0].x - dotPosHalfInterval) * 0.1f;
        //testPos.z *= 0.1f;
        //drawWall(wallVAO, planeShader, fpsCamera, testPos, glm::vec3(1.0f, 3.0f, 1.0f));

        for (Wall wall : wallVector) {
            drawWall(wallVAO, planeShader, fpsCamera, wall.getModel());
        }


        for (glm::vec3 pos : worldPosDots) {
            drawDot(dotVAO, dotShader, fpsCamera, glm::vec3(pos.x * 0.1f, 0.125f, pos.z * 0.1f), glm::vec3(0.25f, 0.25f, 0.25f));
        }

        // Buffer and Input Event
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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
}
