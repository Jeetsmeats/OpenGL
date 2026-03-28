#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#define FRAGMENT_SHADER_PATH PROJECT_ROOT "/shaders/shader.fs"
#define VERTEX_SHADER_PATH PROJECT_ROOT "/shaders/shader.vs"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define STB_IMAGE_IMPLEMENTATION

#include "Shader.h"
#include "stb_image.h"

using namespace glm;
using namespace std;

const string containerPath = PROJECT_ROOT "/resources/container.jpg";
const string awesomeFacePath = PROJECT_ROOT "/resources/awesomeface.png";

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

int main() {

  cout <<  containerPath << " " << awesomeFacePath << endl;

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

  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;

  // set of vertices
  float vertices[] = {
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  Shader shader = Shader(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);

  unsigned int VBO, VAO;

  // generate buffer objects and vertex array object
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(
      0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
      (void *)0); // set the vertex attribute pointer to the buffer object
  glEnableVertexAttribArray(0);


  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  unsigned int texture1, texture2;

  // load textures
  int width, height, nrChannels;
  unsigned char *data = stbi_load(containerPath.c_str(), &width, &height, &nrChannels, 0);

  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);

  // texture wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_REPEAT); // set texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR); // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
  } else {
    std::cout << "Failed to load texture1: " << stbi_failure_reason() << std::endl;
    return 1;
  }

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);

  // texture wrapping
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                  GL_REPEAT); // set texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR); // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(true); // flip the texture on load
  unsigned char *data2 = stbi_load(awesomeFacePath.c_str(), &width, &height, &nrChannels, 0);
  if (data2) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data2);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data2); // free image data after generating texture
  } else {
    std::cout << "Failed to load texture2" << std::endl;
    return 1;
  }

  mat4 model = mat4(1.0f);
  mat4 view = mat4(1.0f);
  mat4 projection = mat4(1.0f);

  view = translate(view, vec3(0.0f, 0.0f, -3.0f));
  projection = perspective(radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);

  shader.use();
  shader.setInt("ourTexture", 0);
  shader.setInt("ourTexture2", 1);
  shader.setMat4("model", model);
  shader.setMat4("view", view);
  shader.setMat4("projection", projection);

  float prevTime = glfwGetTime();
  float deltaTime = 0.0f;
  float angle = 0.0f;
  float rotationSpeed = radians(180.0f); // 90 degrees per second

  glEnable(GL_DEPTH_TEST); // enable depth testing

  while (!glfwWindowShouldClose(window)) {

    deltaTime = glfwGetTime() - prevTime;
    prevTime = glfwGetTime();

    // check if the escape key was pressed or the window was closed
    processInput(window);

    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    model = rotate(model, (float) deltaTime * rotationSpeed, vec3(0.5f, 1.0f, 0.0f));
    shader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36); // update buffers
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteTextures(1, &texture1);
  glDeleteTextures(1, &texture2);

  shader.deleteShader();

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
