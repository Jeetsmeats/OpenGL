#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:

	// Camera attributes
	glm::vec3 CameraPos;
	glm::vec3 CameraFront; 
	glm::vec3 CameraUp;
	glm::vec3 CameraRight;
	glm::vec3 WorldUp;

	/// @brief Constructs a new Camera object
	/// @param position The initial position of the camera
	/// @param up The up vector of the camera
	/// @param yaw The initial yaw angle of the camera
	/// @param pitch The initial pitch angle of the camera
	/// @param lastX The initial x-coordinate of the mouse cursor
	/// @param lastY The initial y-coordinate of the mouse cursor
	Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch, float lastX, float lastY);
	
	/// @brief Handles mouse movement input
	/// @param window Pointer to the GLFW window 
	/// @param xpos The new x-coordinate of the mouse cursor
	/// @param ypos The new y-coordinate of the mouse cursor
	void mouseCallback(GLFWwindow* window, double xpos, double ypos);

	/// @brief Handles mouse scroll input for zooming
	/// @param window Pointer to the GLFW window
	/// @param xoffset The scroll offset in the x-direction
	/// @param yoffset The scroll offset in the y-direction
	void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	/// @brief Calculates and returns the view matrix using the LookAt function
	glm::mat4 calculateLookAt();

	/// @brief Returns the current zoom (FOV) value
	float getZoom() const { return _zoom; }

private:

	// Last mouse positions
	float _lastX;
	float _lastY;

	// Euler angles
	float _yaw;
	float _pitch;

	// Camera options
	float _movementSpeed;
	float _mouseSensitivity;
	float _zoom;

	// Mouse state on start up
	bool _firstMouse = true;

};
#endif