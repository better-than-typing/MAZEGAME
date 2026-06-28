//
// Created by EyesightsX on 2026-06-27.
//

#ifndef ENTITY_H
#define ENTITY_H
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "camera.h"

struct Entity {

    // World Pos
    glm::vec3 position;
    glm::vec3 size;
    glm::vec3 rotationAxis;
    float rotationAngle;

    Camera &fpsCamera;

    [[nodiscard]] glm::mat4 getModel() const {
        auto model = glm::translate(glm::mat4(1.0f), position);

        if (rotationAxis != glm::vec3(0.0f, 0.0f, 0.0f)) {
            model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
        }

        model = glm::scale(model, size);

        return model;
    }

    [[nodiscard]] glm::mat4 getViewModel() const {

        glm::vec3 toCameraDir = glm::normalize(fpsCamera.Position - position);
        toCameraDir.y = 0.0f;

        float angle = atan2(toCameraDir.x, toCameraDir.z);

        auto model = glm::mat4(1.0f);
        model = glm::rotate(model, -angle, glm::vec3(0.0f, 1.0f, 0.0f));
        return model;
    }
};

#endif //MAZEGAME_ENTITY_H