#include "../Headers/debug.h"

#include <cstdlib>
#include <format>
#include <string>

#include "../External Headers/imgui.h"
#include "../External Headers/imgui_impl_glfw.h"
#include "../External Headers/imgui_impl_opengl3.h"


void interfaceInit(GLFWwindow *window) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
}


void showDebugInterface(bool& showDevUI, Camera &playerCamera, float deltaTime) {

    ImGui::SetNextWindowSize(ImVec2(400, 400));
    if (ImGui::Begin("Dev Mode", &showDevUI)) {

        // Wireframe
        ImGui::Checkbox("Wireframe View", &showWireframe);

        // Flymode
        ImGui::Checkbox("Fly Mode", &flyMode);

        // Collision
        std::string collision_wall_index = std::format("Collision Index {}", playerTouchingWallIndex);
        ImGui::Text(collision_wall_index.c_str());

        // Fps
        std::string fps_string = std::format("Frames Per Sec: {}", 1.0f / deltaTime);
        ImGui::Text(fps_string.c_str());

        // Camera Pos
        std::string pos_vec3_string = std::format("Camera Position: ({}, {}, {})", playerCamera.Position.x, playerCamera.Position.y, playerCamera.Position.z);
        ImGui::Text(pos_vec3_string.c_str());

        // Camera Look Vector
        std::string look_vec3_string = std::format("Look Vector: ({}, {}, {})", playerCamera.Front.x, playerCamera.Front.y, playerCamera.Front.z);
        ImGui::Text(look_vec3_string.c_str());

    }

    ImGui::End();
}

void handleDebug() {

}