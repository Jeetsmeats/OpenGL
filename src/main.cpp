/**
 * @Author: Gunjeet Singh
 * @Date:   2025-01-29 18:39:11
 * @Last Modified by:   Your name
 * @Last Modified time: 2025-02-12 10:33:48
 */
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "callbacks.h"
#include "key_input.h"
#include "shader.h"

using namespace std;

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800

int main(int argc, char **argv)
{
    // initialise GLFW settings
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create window
    GLFWwindow* window = glfwCreateWindow(SCREEN_HEIGHT, SCREEN_WIDTH, "LearnOpenGL", NULL, NULL);
    
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    if (window == NULL) // window not found
    {                           
        cout << "Failed to create a GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    // create context for current calling thread
    glfwMakeContextCurrent(window);

    // set the frame buffer resizing callback function to window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) 
    {
        cout << "Failed to initialise GLAD" << endl;
        return -1;
    }
    
    // set viewport parameters
    glViewport(0, 0, 800, 600);

    // create the shader program
    Shader ourShader("shaders/shader.vs", "shaders/shader.fs");
    ourShader.use();                        // activate program

    float vertices[] = {
        // First triangle
        -0.5f,  -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 
        0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
    };

    // triangle indices
    unsigned int indices[] = {
        0, 1, 2
    };
    
    // Vertex buffer and and element buffer
    unsigned int VBO;
    // Vertex array object
    unsigned int VAO;

    // generate vertex buffer
    glGenBuffers(1, &VBO);
    
    // generate vertex array object
    glGenVertexArrays(1, &VAO);

    // bind vertex array
    glBindVertexArray(VAO);
    
    // copy vertices into buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
    // set vertex attribute location
    glEnableVertexAttribArray(0);
    // vertex attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    // set vertex attribute location
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);

    // Wireframe Mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  
    // Default Mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    cout << "Maximum supported vertex attributes: " << nrAttributes << endl;

    ourShader.use();
    
    float offset = 0.5f;
    ourShader.setFloat("xOffset", offset);
    
    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window);           // key inputs
        
        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);        // swap buffers
        glfwPollEvents();               // poll for events
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    ourShader.deleteShader();

    glfwTerminate();
    return 0;
}
