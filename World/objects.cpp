#include "../Headers/objects.h"
#include "../Headers/camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "../Headers/mazegen.h"
// Was 36 floats (6 per vertex, no texcoords). Now 48 (8 per vertex).
float texSize = planeSize / 10.0f;
float planeVertices[66] = {
    // positions          // colors          // texcoords                   // normals
    -0.5f, 0.0f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,                     0.0f, 1.0f, 0.0f,
     0.5f, 0.0f,  0.5f,  1.0f, 1.0f, 1.0f,  1.0f * texSize, 0.0f,           0.0f, 1.0f, 0.0f,
     0.5f, 0.0f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f * texSize, 1.0f * texSize, 0.0f, 1.0f, 0.0f,

     0.5f, 0.0f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f * texSize, 1.0f * texSize, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.0f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 0.0f,                     0.0f, 1.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f * texSize,           0.0f, 1.0f, 0.0f
};

float wallVertices[48] = {
    // first triangle
    // positions          // colors           // texture coords
    -0.5f,  0.0f,  0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // bottom-left
     0.5f,  0.0f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // bottom-right
     0.5f,  0.5f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top-right

    // second triangle
    // positions          // colors           // texture coords
     0.5f,  0.5f,  0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top-right
    -0.5f,  0.5f,  0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 1.0f,   // top-left
    -0.5f,  0.0f,  0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f    // bottom-left
};

const static std::vector<float> cubeVertices =
{
    -0.5f,0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f,0.5f,-0.5f,

    -0.5f,0.5f,0.5f,
    -0.5f,-0.5f,0.5f,
    0.5f,-0.5f,0.5f,
    0.5f,0.5f,0.5f,

    0.5f,0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,0.5f,
    0.5f,0.5f,0.5f,

    -0.5f,0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,0.5f,
    -0.5f,0.5f,0.5f,

    -0.5f,0.5f,0.5f,
    -0.5f,0.5f,-0.5f,
    0.5f,0.5f,-0.5f,
    0.5f,0.5f,0.5f,

    -0.5f,-0.5f,0.5f,
    -0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,-0.5f,
    0.5f,-0.5f,0.5f
};

const static std::vector<int> cubeIndices =
{
    0,1,3,
    3,1,2,
    4,5,7,
    7,5,6,
    8,9,11,
    11,9,10,
    12,13,15,
    15,13,14,
    16,17,19,
    19,17,18,
    20,21,23,
    23,21,22
};

unsigned int registerCube() {
    unsigned int VAO, VBO, EBO;

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(float), cubeVertices.data(), GL_STATIC_DRAW);

    // EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubeIndices.size() * sizeof(int), cubeIndices.data(), GL_STATIC_DRAW);

    // Pos Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);

    return VAO;
}

std::vector<glm::vec3> getDotsWorldPosVector(int dotNumberOneSided, float planeWidthorLength, glm::vec3 planeBottomRightPoint) {
    std::vector<glm::vec3> dot_position_vector;

    if (dotNumberOneSided < 2) {
        std::cout << "Grid Size Too Small, Exiting...\n";
        glfwTerminate();
        return {};
    }

    float dotPosInterval = planeWidthorLength / static_cast<float>(--dotNumberOneSided);
    dotPosHalfInterval = dotPosInterval / 2.0f;
    int totalDots = dotNumberOneSided * dotNumberOneSided;

    float dotXCurrPos = planeBottomRightPoint.x;
    float dotZCurrPos = planeBottomRightPoint.z;

    int dotCount = 1;
    for (int dotZ = 0; dotZ <= dotNumberOneSided; dotZ++) {
        for (int dotX = 0; dotX <= dotNumberOneSided; dotX++) {
            glm::vec3 dotPosition{dotXCurrPos, 0.0f, dotZCurrPos};

            //std::cout << "Dot" << dotCount << ": " << dotPosition.x << ", " << dotPosition.z << std::endl;

            dot_position_vector.push_back(dotPosition);
            dotXCurrPos += dotPosInterval;
            dotCount++;
        }
        dotXCurrPos = planeBottomRightPoint.x;
        dotZCurrPos -= dotPosInterval;

    }

    return dot_position_vector;
}

unsigned int registerPlane() {

    unsigned int VAO, VBO;

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    // Position Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);

    // Color Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture Coord Attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Normals Attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*) (8 * sizeof(float)));
    glEnableVertexAttribArray(3);

    return VAO;
}

unsigned int registerWall() {
    unsigned int VAO, VBO;

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wallVertices), wallVertices, GL_STATIC_DRAW);

    // Position Attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);

    // Color Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture Attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    return VAO;
}

void drawCube(unsigned int cubeVAO, Shader& cubeShader, Camera& fpsCamera, glm::vec3 worldPos, glm::vec3 sizeVec) {
    cubeShader.use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(fpsCamera.Zoom), 1280.0f / 800.0f, 0.1f, 100.0f);
    glm::mat4 view = fpsCamera.GetViewMatrix();

    cubeShader.setMat4("projection", projection);
    cubeShader.setMat4("view", view);

    // world transformation

    glm::mat4 model = glm::translate(glm::mat4(1.0f), worldPos);
    glm::vec3 scale = sizeVec;
    model = glm::scale(model, scale);

    cubeShader.setMat4("model", model);
    cubeShader.setVec3("dotColor", glm::vec3(1.0f, 1.0f, 1.0f));

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, cubeIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void drawPlane(unsigned int planeVAO, Shader& planeShader, Camera& fpsCamera, unsigned int planeTexture) {
    planeShader.use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(fpsCamera.Zoom), 1280.0f / 800.0f, 0.1f, 100.0f);
    glm::mat4 view = fpsCamera.GetViewMatrix();

    planeShader.setMat4("projection", projection);
    planeShader.setMat4("view", view);


    // world transformation
    glm::vec3 scale = glm::vec3(planeSize / 8.0f, 1.0f, planeSize / 8.0f);
    glm::mat4 model = glm::scale(glm::mat4(1.0f), scale);

    planeShader.setMat4("model", model);

    //TODO MAGIC NUMBER
    planeShader.setVec3("lightPos", glm::vec3(0.0f, 0.5f, 0.0f));
    planeShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    planeShader.setVec3("viewPos", fpsCamera.Position);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, planeTexture);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}



void drawWall(unsigned int wallVAO, Shader& wallShader, Camera& fpsCamera, const glm::mat4& model, unsigned int wallTexture) {
    wallShader.use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(fpsCamera.Zoom), 1280.0f / 800.0f, 0.1f, 100.0f);
    glm::mat4 view = fpsCamera.GetViewMatrix();

    // Vertex Shader
    wallShader.setMat4("projection", projection);
    wallShader.setMat4("view", view);
    wallShader.setMat4("model", model);

    // Fragment Shader
    wallShader.setVec3("wallColor", glm::vec3(0.0f, 0.0f, 0.5f));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, wallTexture);

    glBindVertexArray(wallVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
