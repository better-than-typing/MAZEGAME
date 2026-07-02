#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <random>

#include "External Headers/imgui.h"
#include "External Headers/imgui_impl_glfw.h"
#include "External Headers/imgui_impl_opengl3.h"
#include "Headers/shader.h"
#include "Headers/objects.h"
#include "Headers/camera.h"
#include "Headers/mazegen.h"
#include "Headers/input.h"
#include "Headers/debug.h"
#include "Headers/post_processing.h"

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

#include <string>

inline std::string assetPath(const std::string& relative) {
    return std::string(ROOT_DIR) + "/" + relative;
}

int main() {

    // Init window and UI
    initWindow();
    interfaceInit(window);

    // VAOs init
    unsigned int planeVAO = registerPlane();
    unsigned int wallVAO = registerWall();
    unsigned int dotVAO = registerCube();
    unsigned int screenQuadVAO = registerScreenQuad();
    unsigned int lightCubeVAO = registerCube();

    // Textures
    unsigned int groundTexture = loadTexture(assetPath("Assets/floor_wood_planks.png").c_str());
    unsigned int roofTexture   = loadTexture(assetPath("Assets/floor_stone_sand_random.png").c_str());
    unsigned int wallTexture   = loadTexture(assetPath("Assets/planeTexture.png").c_str());

    // Shaders
    Shader planeShader(assetPath("Shaders/Plane/planeVS.glsl").c_str(), assetPath("Shaders/Plane/planeFS.glsl").c_str());
    Shader roofShader(assetPath("Shaders/Roof/roofVS.glsl").c_str(),   assetPath("Shaders/Roof/roofFS.glsl").c_str());
    Shader dotShader(assetPath("Shaders/Dot/dotVS.glsl").c_str(),     assetPath("Shaders/Dot/dotFS.glsl").c_str());
    Shader lightShader(assetPath("Shaders/Light/lightVS.glsl").c_str(), assetPath("Shaders/Light/lightFS.glsl").c_str());
    Shader wallShader(assetPath("Shaders/Wall/wallVS.glsl").c_str(),   assetPath("Shaders/Wall/wallFS.glsl").c_str());
    Shader wallExitShader(assetPath("Shaders/Wall/wallVS.glsl").c_str(), assetPath("Shaders/Wall/wallExitFs.glsl").c_str());
    Shader screenShader(assetPath("Shaders/Screen/screenVS.glsl").c_str(), assetPath("Shaders/Screen/screenFS.glsl").c_str());

    // Frame Buffer Init
    unsigned int frameBuffer;
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH,SCREEN_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // World Generation init
    Maze::generateMaze();

    int exitWallIndex = getExitWallIndex();

    while (!glfwWindowShouldClose(window)) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //showDebugInterface(showDevUI, fpsCamera, deltaTime);

        // Frame Handling
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input
        processInput(window);
        togglePathCubes(window);
        toggleCursor(window);

        // Frame Buffer
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
        glEnable(GL_DEPTH_TEST);

        // Debug
        if (showWireframe) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }

        // Background
        glClearColor(0.0f, 0.0f, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw Objects
        drawPlane(planeVAO, roofShader, fpsCamera, roofTexture, glm::vec3(0.0f, 1.2f, 0.0f), true);
        drawPlane(planeVAO, planeShader, fpsCamera, groundTexture, glm::vec3(0.0f, 0.0f, 0.0f), false);

        int i = 0;
        for (Wall wall : currentWallVector) {

            if (i == exitWallIndex) {
                drawWall(wallVAO, wallExitShader, fpsCamera, wall.getModel(), wallTexture);
            } else {
                drawWall(wallVAO, wallShader, fpsCamera, wall.getModel(), wallTexture);
            }
            i++;
        }

        if (cubePathShown) {
            for (glm::vec3 pos : worldPosDots) {
                drawCube(dotVAO, lightShader, fpsCamera, glm::vec3(pos.x * 0.1f, 0.125f, pos.z * 0.1f), glm::vec3(0.25f, 0.25f, 0.25f));
            }
        }

        if (isPlayerAtExit(exitWallIndex)) {
            break;
        }

        // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // clear all relevant buffers
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader.use();
        screenShader.setFloat("time", glfwGetTime());
        screenShader.setFloat("glitchIntensity", 0.5f);

        glBindVertexArray(screenQuadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);

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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    setIcon(window, assetPath("Assets/Pfp.png").c_str());
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
