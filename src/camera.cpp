#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"

glm::mat4 Camera::calculateLookAt() {

    glm::vec3 cameraTarget = cameraPos + cameraFront;
    glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    cameraRight = glm::normalize(glm::cross(worldUp, cameraDirection));
    cameraUp = glm::cross(cameraDirection, cameraRight);

    glm::mat4 rot = glm::mat4(1.0f);
    glm::mat4 trans = glm::mat4(1.0f);

    // row 1 - rotation matrix
    rot[0][0] = cameraRight.x;
    rot[1][0] = cameraRight.y;
    rot[2][0] = cameraRight.z;
    // row 2 - rotation matrix
    rot[0][1] = cameraUp.x;
    rot[1][1] = cameraUp.y;
    rot[2][1] = cameraUp.z;
    // row 3 - rotation matrix
    rot[0][2] = cameraDirection.x;
    rot[1][2] = cameraDirection.y;
    rot[2][2] = cameraDirection.z;

    // translation matrix
    trans[3][0] = -cameraPos.x;
    trans[3][1] = -cameraPos.y;
    trans[3][2] = -cameraPos.z;

    return rot * trans;
}

float Camera::getZoom() {
    return zoom;
}

void Camera::cameraZoom(float zoomOffset) {
    zoom -= (float) zoomOffset;
    if (zoom < 1.0f) {
        zoom = 1.0f;
    }
    if (zoom > 45.0f) {
        zoom = 45.0f;
    }
}

void Camera::moveCamera(CAMERA_MOVEMENT cmd, float deltaTime) {

    const float cameraSpeed = accel * deltaTime;

    switch (cmd) {
        case FORWARD:
            cameraPos += cameraSpeed * cameraFront;
            break;
        case BACKWARD:
            cameraPos -= cameraSpeed * cameraFront;
            break;
        case LEFT:
            cameraPos -= cameraSpeed * cameraRight;
            break;
        case RIGHT:
            cameraPos += cameraSpeed * cameraRight;
            break;
        default:
            break;
    }
}

void Camera::orientCamera(double lastX, double lastY, double xPos, double yPos) {
    
    // calculate offset
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;               // not-inverted

    // apply camera sensitivity
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    // update camera orientation
    yaw += xOffset;
    pitch += yOffset;

    // constraints
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    
    glm::vec3 front = glm::vec3(
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))
    );
    cameraFront = glm::normalize(front);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}