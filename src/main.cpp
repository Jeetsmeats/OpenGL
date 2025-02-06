/**
 * @Author: Your name
 * @Date:   2025-01-29 18:39:11
 * @Last Modified by:   Your name
 * @Last Modified time: 2025-02-06 17:27:45
 */
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "callbacks.h"
#include "key_input.h"

using namespace std;

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

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

    if (window == NULL) {                           // window not found
        cout << "Failed to create a GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    // create context for current calling thread
    glfwMakeContextCurrent(window);

    // set the frame buffer resizing callback function to window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        cout << "Failed to initialise GLAD" << endl;
        return -1;
    }
    
    // set viewport parameters
    glViewport(0, 0, 800, 600);

    // create the vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // attach vertex source shader to vertex shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // compile shader
    glCompileShader(vertexShader);
    
    // check vertex shader compilation for errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {                     // return failed status

        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout <<  "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
            infoLog << endl;
    }

    // create the fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // attached fragment shader source to fragment shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // compile the fragment shader
    glCompileShader(fragmentShader);

    // check fragment shader compilation for errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {                         // return failed status

        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }

    // set up the shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    
    // attach shaders to the program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // link shaders
    glLinkProgram(shaderProgram);
    
    //check linking error status
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {                             // return failed status
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }

    // activate shader program
    glUseProgram(shaderProgram);

    // delete shader objectss
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    float vertices[] = {
        // First triangle
        0.0f,  0.5f,  0.0f,  
        0.5f, 0.0f,  0.0f,  
        -0.5f, 0.0f,  0.0f,  // Bottom-left
        0.0f, 0.0f, 0.0f     // Top-left
    };

    // triangle indices
    unsigned int indices[] = {
        0, 1, 3,
        0, 2, 3
    };

    // Vertex buffer and and element buffer
    unsigned int VBO, EBO;
    // Vertex array object
    unsigned int VAO;

    // generate vertex buffer
    glGenBuffers(1, &VBO);
    // generate element buffer
    glGenBuffers(1, &EBO);
    
    // generate vertex array object
    glGenVertexArrays(1, &VAO);

    // bind vertex array
    glBindVertexArray(VAO);
    
    // copy vertices into buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    // copy indices into buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                GL_STATIC_DRAW);

    // vertex attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    // set vertex attribute location
    glEnableVertexAttribArray(0);
    
    // bind array object to buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Wireframe Mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  
    // Default Mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    while (!glfwWindowShouldClose(window)) {

        processInput(window);           // key inputs
        
        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);        // swap buffers
        glfwPollEvents();               // poll for events
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
