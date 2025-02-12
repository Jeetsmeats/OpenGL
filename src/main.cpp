/**
 * @Author: Gunjeet Singh
 * @Date:   2025-01-29 18:39:11
 * @Last Modified by:   Your name
 * @Last Modified time: 2025-02-12 15:53:59
 */
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// include member functions/classes
#include "callbacks.h"
#include "key_input.h"
#include "shader.h"

// include images
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

    // vertex coords
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f,-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
    };

    // texture coords
    float textCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f, 
        0.5f, 1.0f
    };
    
    // triangle indices
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // Vertex buffer, array object and element buffer
    unsigned int VBO, VAO, EBO;

    // generate vertex array object
    glGenVertexArrays(1, &VAO);
    // generate vertex buffer
    glGenBuffers(1, &VBO);
    // generate element buffer
    glGenBuffers(1, &EBO);

    // bind vertex array
    glBindVertexArray(VAO);
    
    // copy vertices into buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // copy data into buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // copy vertices into buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // copy data into buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // define vertex attribute - position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    // define vertex attribute - color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // define vertex attributes - texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // define & generate textures
    unsigned int texture;
    glGenTextures(1, &texture);
    
    // bind texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // set texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // define texture parameters
    int width, height, nrChannels;
    unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        // generate texture w/ image
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
    } else 
    {
        cout << "Failed to load the texture" << endl;
    }
    
    // free image memory
    stbi_image_free(data);
    
    // Wireframe Mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  
    // Default Mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // check number of attributes
    // int nrAttributes;
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    // cout << "Maximum supported vertex attributes: " << nrAttributes << endl;
    
    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window);           // key inputs
        
        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind texture
        glBindTexture(GL_TEXTURE_2D, texture);

        // render container
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);        // swap buffers
        glfwPollEvents();               // poll for events
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    ourShader.deleteShader();

    glfwTerminate();
    return 0;
}
