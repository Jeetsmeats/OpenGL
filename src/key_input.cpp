/**
 * @Author: Gunjeet Singh
 * @Date:   2025-02-02 19:23:11
 * @Last Modified by:   Your name
 * @Last Modified time: 2025-02-11 17:03:24
 */
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