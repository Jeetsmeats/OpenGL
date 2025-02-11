/**
 * @Author: Gunjeet Singh
 * @Date:   2025-02-02 17:10:41
 * @Last Modified by:   Your name
 * @Last Modified time: 2025-02-11 17:03:18
 */
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "callbacks.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}