#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CAMERA_MOVEMENT
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// camera constants
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED_CONSTANT = 5.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

class Camera {
private:

    // camera global variables
    glm::vec3 cameraPos;
    glm::vec3 cameraFront;
    glm::vec3 cameraRight;
    glm::vec3 cameraUp;

    // world attributes
    glm::vec3 worldUp;
    glm::mat4 lookAt;

    // angle with respect to world coordinate system
    float yaw;
    float pitch;
    
    // camera constants
    float speedConstant;
    float sensitivity;
    float zoom;

public:
    Camera(glm::vec3 cPos, glm::vec3 wUp = WORLD_UP,
           float yaw = YAW, float pitch = PITCH, float speedConstant = SPEED_CONSTANT, float sensitivity = SENSITIVITY, float zoom = ZOOM) : 
           cameraPos(cPos), worldUp(wUp), cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),
           yaw(yaw), pitch(pitch), speedConstant(speedConstant), sensitivity(sensitivity), zoom(zoom) {};
    
    /// @brief get the field of view
    /// @return field of view value
    float getZoom() const;

    /// @brief get the camera position
    /// @return global camera position
    glm::vec3 getCameraPosition() const;

    /// @brief get the camera front
    /// @return global camera front
    glm::vec3 getCameraFront() const;

    /// @brief calculate the lookAt matrix
    /// @param cTarget camera target
    /// @return lookAt matrix
    glm::mat4 calculateLookAt();

    /// @brief zoom camera functions
    /// @param scroll offset value
    void cameraZoom(float zoomOffset);

    /// @brief orient the camera based on mouse position
    /// @param lastX previous mouse x position
    /// @param lastY previous mouse y position
    /// @param xPos current mouse x position
    /// @param yPos current mouse y position
    void orientCamera(double lastX, double lastY, double xPos, double yPos);

    /// @brief move the camera using key bindings
    /// @param cmd movement command
    /// @param deltaTime time difference from previous frame to current
    void moveCamera(CAMERA_MOVEMENT cmd, float deltaTime);

    /// @brief update the camera coordinate system
    void updateCameraVectors();
};

#endif
