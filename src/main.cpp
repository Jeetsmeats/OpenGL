#include <iostream>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// include member functions/classes
#include "shader.h"
#include "camera.h"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 800

// resource paths
const filesystem::path rootPath = filesystem::current_path();
const filesystem::path texturePath = rootPath / "resources" / "textures";

// global camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

// colors
const glm::vec3 coral(1.0f, 0.5f, 0.31f);                 
const glm::vec3 white(1.0f, 1.0f, 1.0f);
const glm::vec3 green(0.0f, 1.0f, 0.0f);

glm::vec3 lightSource = green;
glm::vec3 toyColor = coral;
glm::vec3 result = lightSource * toyColor;

bool firstMouse = true;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float lastX = (float) SCREEN_WIDTH / 2;
float lastY = (float) SCREEN_HEIGHT / 2;

// light source
glm::vec3 lightPos(1.2f, 1.0f, 3.0f);

// declarations
void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xPos, double yPos);
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset);
unsigned int loadTexture(const char *filePath);

int main(int argc, char **argv)
{
    filesystem::path imagePath = texturePath / "container2.png";
    filesystem::path specImagePath = texturePath / "container2_specular.png";
    string imagePathStr = imagePath.string();
    string specImagePathStr = specImagePath.string();

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // set callback functions
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) 
    {
        cout << "Failed to initialise GLAD" << endl;
        return -1;
    }
    
    // set viewport parameters
    glViewport(0, 0, 800, 600);

    // create the shader program
    Shader shader("shaders/shader.vs", "shaders/shader.fs");
    Shader lightSourceShader("shaders/lightShader.vs", "shaders/lightShader.fs");

    // cube vertices
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    unsigned int texture = loadTexture(imagePathStr.c_str());
    unsigned int specTexture = loadTexture(specImagePathStr.c_str());

    // Vertex buffer, array object and element buffer
    unsigned int VBO, objectVAO;

    glGenVertexArrays(1, &objectVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(objectVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // define vertex attribute - position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // positions all containers
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // set object shader constants
    shader.use();
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);

    shader.setFloat("light.constant", 1.0f);
    shader.setFloat("light.linear", 0.09f);
    shader.setFloat("light.quadratic", 0.032f);
    shader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    shader.setFloat("light.outerCutOff", glm::cos(glm::radians(15.0f)));

    // light objectVAO
    lightSourceShader.use();

    unsigned int lightObjectVAO;
    glGenVertexArrays(1, &lightObjectVAO);

    // bind vertex array
    glBindVertexArray(lightObjectVAO);

    // bind vertex object buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // Wireframe Mode
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  
    // Default Mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    // view and projection matrices
    glm::mat4 view;
    glm::mat4 model;
    glm::mat4 projection;

    // check number of attributes
    // int nrAttributes;
    // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    // cout << "Maximum supported vertex attributes: " << nrAttributes << endl;
    
    while (!glfwWindowShouldClose(window)) 
    {
        processInput(window); // key inputs
        
        // rendering commands
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        shader.setVec3("light.position", camera.getCameraPosition());
        shader.setVec3("light.direction", camera.getCameraFront());
        shader.setVec3("viewPos", camera.getCameraPosition());
        
        // light properties
        shader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        shader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        shader.setFloat("material.shininess", 32.0f);

        view = camera.calculateLookAt();
        projection = glm::perspective(glm::radians(camera.getZoom()),
                 (float) SCREEN_HEIGHT / (float) SCREEN_WIDTH, 0.1f, 100.0f);

        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        // shader.setVec3("light.direction", glm::vec3(-0.2f, -1.0f, -0.3f));

        // bind texture
        glActiveTexture(GL_TEXTURE0); // texture unit 0
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specTexture);
        glBindVertexArray(objectVAO);

        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);  
        }

        // light source
        lightSourceShader.use();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        
        // transformation matrix uniform locations
        lightSourceShader.setMat4("view", view);
        lightSourceShader.setMat4("projection", projection);
        lightSourceShader.setMat4("model", model);

        lightSourceShader.setVec3("lightColor", glm::vec3(1.0f));

        glBindVertexArray(lightObjectVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);  

        glfwSwapBuffers(window); // swap buffers
        glfwPollEvents();        // poll for events
    }

    glDeleteVertexArrays(1, &objectVAO);
    glDeleteVertexArrays(1, &lightObjectVAO);
    glDeleteBuffers(1, &VBO);
    shader.deleteShader();

    glfwTerminate();
    return 0;
}

/// @brief process keyboard events
/// @param window game window
void processInput(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {                // escape to close

        glfwSetWindowShouldClose(window, true);             // close window
    }

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.moveCamera(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.moveCamera(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.moveCamera(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.moveCamera(RIGHT, deltaTime);
    }
}

/// @brief window size adjustment callback
/// @param window app window
/// @param width app width
/// @param height app height
void framebuffer_size_callback(GLFWwindow *window, int width, int height) 
{
    glViewport(0, 0, width, height);
}

/// @brief mouse movement callback
/// @param window app window
/// @param xPos mouse x position
/// @param yPos mouse y position
void mouse_callback(GLFWwindow *window, double xPos, double yPos) {

    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    camera.orientCamera(lastX, lastY, xPos, yPos);
    lastX = xPos;
    lastY = yPos;
}

/// @brief mouse scroll callback
/// @param window app window
/// @param xOffset x scroll
/// @param yOffset y scroll - main scroll
void scroll_callback(GLFWwindow *window, double xOffset, double yOffset) {

    camera.cameraZoom((float) yOffset);
}

unsigned int loadTexture(const char* filePath) {

    unsigned int texture;
    glGenTextures(1, &texture);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(filePath, &width, &height, &nrChannels, 0);

    if (data) {

        GLenum format;
        
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    } else {
        cout << "failed to load texture" << endl;
    }
    stbi_image_free(data);

    return texture;
}