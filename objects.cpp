#include "Headers/objects.h"
#include "Headers/camera.h"

#include <glad/glad.h>

float planeVertices[36] = {
    // first triangle
    // positions          // colors
    -0.5f, 0.0f,  0.5f,  1.0f, 1.0f, 1.0f,
     0.5f, 0.0f,  0.5f,  1.0f, 1.0f, 1.0f,
     0.5f, 0.0f, -0.5f,  1.0f, 1.0f, 1.0f,

    // second triangle
    // positions          // colors
     0.5f, 0.0f, -0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f, 0.0f,  0.5f,  1.0f, 1.0f, 1.0f,
    -0.5f, 0.0f,  -0.5f,  1.0f, 1.0f, 1.0f,
};

float wallVertices[36] = {
    // first triangle
    // positions          // colors
    -0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
     0.5f, 0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
     0.5f, 0.5f,  0.0f,  1.0f, 0.0f, 0.0f,

    // second triangle
    // positions          // colors
     0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    -0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
};

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);

    // Color Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);

    // Color Attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return VAO;
}

void drawPlane(unsigned int planeVAO, Shader& planeShader, Camera& fpsCamera) {
    planeShader.use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(fpsCamera.Zoom), 1280.0f / 800.0f, 0.1f, 100.0f);
    glm::mat4 view = fpsCamera.GetViewMatrix();

    planeShader.setMat4("projection", projection);
    planeShader.setMat4("view", view);


    // world transformation
    glm::vec3 scale = glm::vec3(10.0f, 10.0f, 10.0f);
    glm::mat4 model = glm::scale(glm::mat4(1.0f), scale);

    planeShader.setMat4("model", model);

    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}



void drawWall(unsigned int wallVAO, Shader& wallShader, Camera& fpsCamera, glm::vec3 worldPos, glm::vec3 sizeVec) {
    wallShader.use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(fpsCamera.Zoom), 1280.0f / 800.0f, 0.1f, 100.0f);
    glm::mat4 view = fpsCamera.GetViewMatrix();

    wallShader.setMat4("projection", projection);
    wallShader.setMat4("view", view);

    // world transformation

    glm::mat4 model = glm::translate(glm::mat4(1.0f), worldPos);
    glm::vec3 scale = sizeVec;
    model = glm::scale(model, scale);

    wallShader.setMat4("model", model);

    glBindVertexArray(wallVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
