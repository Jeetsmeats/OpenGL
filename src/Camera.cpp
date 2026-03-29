#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch,
				   float lastX, float lastY) 
	: CameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), _movementSpeed(2.5f), _mouseSensitivity(0.1f), _zoom(45.0f) {
	CameraPos = position;
	WorldUp = up;
	_yaw = yaw;
	_pitch = pitch;
	_lastX = lastX;
	_lastY = lastY;

	glm::vec3 direction = glm::vec3(
		cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
		sin(glm::radians(_pitch)),
		sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))
	);
	CameraFront = glm::normalize(direction);
	CameraRight = glm::normalize(glm::cross(CameraFront, WorldUp));
	CameraUp = glm::normalize(glm::cross(CameraRight, CameraFront));
}

void Camera::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (_firstMouse) {
		_lastX = xpos;
		_lastY = ypos;
		_firstMouse = false;
	}

	float xoffset = xpos - _lastX;
	float yoffset = _lastY - ypos;							// reversed since y-coordinates go from bottom to top
	_lastX = xpos;
	_lastY = ypos;

	xoffset *= _mouseSensitivity;
	yoffset *= _mouseSensitivity;
	
	_yaw += xoffset;
	_pitch += yoffset;

	// constraint pitch
	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;

	glm::vec3 direction = glm::vec3(
		cos(glm::radians(_yaw)) * cos(glm::radians(_pitch)),
		sin(glm::radians(_pitch)),
		sin(glm::radians(_yaw)) * cos(glm::radians(_pitch))
	);
	CameraFront = glm::normalize(direction);
	CameraRight = glm::normalize(glm::cross(CameraFront, WorldUp));
	CameraUp = glm::normalize(glm::cross(CameraRight, CameraFront));
}

void Camera::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	_zoom -= (float) yoffset;
	if (_zoom < 1.0f)
		_zoom = 1.0f;
	if (_zoom > 45.0f)
		_zoom = 45.0f; 
}

glm::mat4 Camera::calculateLookAt() {
	return glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
}
