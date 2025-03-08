#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "key_input.h"

/// @brief process keyboard events
/// @param window game window
void processInput(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {                // escape to close

        glfwSetWindowShouldClose(window, true);             // close window
    }
}