#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "callbacks.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
    glViewport(0, 0, width, height);
}