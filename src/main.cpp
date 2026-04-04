#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define FRAGMENT_SHADER_PATH PROJECT_ROOT "/shaders/shader.fs"
#define VERTEX_SHADER_PATH PROJECT_ROOT "/shaders/shader.vs"
#define VERTEX_SHADER_LIGHT_PATH PROJECT_ROOT "/shaders/lightShader.vs"
#define FRAGMENT_SHADER_LIGHT_PATH PROJECT_ROOT "/shaders/lightShader.fs"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define STB_IMAGE_IMPLEMENTATION

#include "Camera.h"
#include "Shader.h"
#include "stb_image.h"

const std::string containerPath = PROJECT_ROOT "/resources/container.jpg";
const std::string awesomeFacePath = PROJECT_ROOT "/resources/awesomeface.png";

glm::vec3 coral(1.0f, 0.5f, 0.31f);
glm::vec3 toyColor(0.0f, 0.5f, 0.31f);
glm::vec3 result = coral * toyColor; 										// component-wise multiplication

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)
};

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// CAMERA SETUP
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f,
              0.0f, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, float deltaTime);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                          "OpenGL Window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // create context for the current calling thread
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR,
                     GLFW_CURSOR_DISABLED); // capture the mouse cursor

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum number of vertex attributes supported: "
              << nrAttributes << std::endl;

    // set of vertices
    float vertices[] = {
		// positions 		// normals 			// texture coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

    Shader shader = Shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    Shader lightShader =
        Shader(VERTEX_SHADER_LIGHT_PATH, FRAGMENT_SHADER_LIGHT_PATH);

    unsigned int VBO, VAO;

    // generate buffer objects and vertex array object
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    //   unsigned int texture1, texture2;

    //   load textures
    //   int width, height, nrChannels;
    //   unsigned char *data = stbi_load(containerPath.c_str(), &width, &height,
    //   &nrChannels, 0);

    //   glGenTextures(1, &texture1);
    //   glBindTexture(GL_TEXTURE_2D, texture1);

    //   texture wrapping
    //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
    //   GL_REPEAT); // set texture wrapping parameters
    //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    //   GL_LINEAR); // set texture filtering parameters
    //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //   if (data) {
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
    //  GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);

    // stbi_image_free(data);
    //   } else {
    // std::cout << "Failed to load texture1: " << stbi_failure_reason() <<
    // std::endl; return 1;
    //   }

    //   glGenTextures(1, &texture2);
    //   glBindTexture(GL_TEXTURE_2D, texture2);

    //   texture wrapping
    //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
    //   GL_REPEAT); // set texture wrapping parameters
    //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    //   GL_LINEAR); // set texture filtering parameters
    //   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //   stbi_set_flip_vertically_on_load(true); // flip the texture on load
    //   unsigned char *data2 = stbi_load(awesomeFacePath.c_str(), &width,
    //   &height, &nrChannels, 0); if (data2) {
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
    //  GL_UNSIGNED_BYTE, data2);
    // glGenerateMipmap(GL_TEXTURE_2D);

    // stbi_image_free(data2); // free image data after generating texture
    //   } else {
    // std::cout << "Failed to load texture2" << std::endl;
    // return 1;
    //   }

    shader.use();

	// set material properties
	shader.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
	shader.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
	shader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setFloat("material.shininess", 32.0f);

	// set light properties
	shader.setVec3("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	shader.setVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
	shader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

    float prevTime = glfwGetTime();
    float deltaTime = 0.0f;
    float angle = 0.0f;
    float rotationSpeed = glm::radians(180.0f); 					// 90 degrees per second
	float radius = 10.0f;

    glEnable(GL_DEPTH_TEST); 										// enable depth testing
    while (!glfwWindowShouldClose(window)) {

        deltaTime = glfwGetTime() - prevTime;
        prevTime = glfwGetTime();

        float cameraAngle = (float)glfwGetTime() * rotationSpeed;
		
        glm::mat4 view = camera.calculateLookAt();
        glm::mat4 perspective = glm::perspective(
            glm::radians(camera.getZoom()),
            (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f
		);

        shader.use();
        shader.setMat4("view", view);
        shader.setMat4("projection", perspective);

        // check if the escape key was pressed or the window was closed
        processInput(window, deltaTime);

        // rendering commands here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 lightPos = glm::vec3(radius * cos(cameraAngle), 1.0f, radius * sin(cameraAngle));

        glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		
        lightShader.use();
        lightShader.setMat4("model", model);
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", perspective);
        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        shader.use();
		shader.setVec3("light.position", lightPos);

		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);

		shader.setVec3("light.ambient", lightColor * glm::vec3(0.2f));
		shader.setVec3("light.diffuse", lightColor * glm::vec3(0.5f));
		shader.setVec3("light.specular", lightColor * glm::vec3(1.0f));
		shader.setVec3("viewPos", camera.CameraPos);
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            shader.setMat4("model", model);
            shader.setMat3("normalMatrix", glm::mat3(glm::transpose(glm::inverse(model))));
            glDrawArrays(GL_TRIANGLES, 0, 36); // update buffers
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &lightVAO);
    //   glDeleteTextures(1, &texture1);
    //   glDeleteTextures(1, &texture2);

    shader.deleteShader();
    lightShader.deleteShader();

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    float cameraSpeed = 10.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.CameraPos += camera.CameraFront * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.CameraPos -= camera.CameraFront * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.CameraPos -=
            glm::normalize(glm::cross(camera.CameraFront, camera.CameraUp)) *
            cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.CameraPos +=
            glm::normalize(glm::cross(camera.CameraFront, camera.CameraUp)) *
            cameraSpeed;
    }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    camera.mouseCallback(window, xpos, ypos);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.scrollCallback(window, xoffset, yoffset);
}