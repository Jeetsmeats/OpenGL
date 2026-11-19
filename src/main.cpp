#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#define FRAGMENT_SHADER_PATH PROJECT_ROOT "/shaders/shader.fs"
#define VERTEX_SHADER_PATH PROJECT_ROOT "/shaders/shader.vs"
#define VERTEX_SHADER_LIGHT_PATH PROJECT_ROOT "/shaders/lightShader.vs"
#define FRAGMENT_SHADER_LIGHT_PATH PROJECT_ROOT "/shaders/lightShader.fs"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define STB_IMAGE_IMPLEMENTATION

#include "Camera.h"
#include "GLDebug.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "stb_image.h"

const std::string containerPath = PROJECT_ROOT "/resources/textures/container2.png";
const std::string containerSpecularPath = PROJECT_ROOT "/resources/textures/container2_specular.png";
const std::string backpackPath = PROJECT_ROOT "/resources/objects/backpack/backpack.obj";

// current framebuffer size, kept up to date so the projection aspect ratio
// follows window resizes instead of being stuck at 800x600
int framebufferWidth = WINDOW_WIDTH;
int framebufferHeight = WINDOW_HEIGHT;

glm::vec3 coral(1.0f, 0.5f, 0.31f);
glm::vec3 toyColor(0.0f, 0.5f, 0.31f);
glm::vec3 result = coral * toyColor; // component-wise multiplication

glm::vec3 cubePositions[] = {
    glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
    glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// CAMERA SETUP
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f,
              0.0f, WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f);

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, float deltaTime);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void configureLitShader(Shader &shader);
bool saveScreenshot(const char *path, int width, int height);

// set by pressing R: force a shader reload even if no file changed
bool reloadRequested = false;

int main(int argc, char **argv) {

  // optional: --capture out.ppm renders a few frames, saves one and exits
  // (handy for checking a change from a script without opening the window)
  // (--frames N picks which frame to save; default 30)
  const char *capturePath = nullptr;
  int captureFrame = 30;
  for (int i = 1; i + 1 < argc; i++) {
    if (std::strcmp(argv[i], "--capture") == 0)
      capturePath = argv[i + 1];
    else if (std::strcmp(argv[i], "--frames") == 0)
      captureFrame = std::max(1, std::atoi(argv[i + 1]));
  }

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); // see GLDebug.h

  GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT,
                                        "OpenGL Window  |  edit shaders/*.fs and save to hot-reload  |  R = reload",
      NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  // create context for the current calling thread
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
  glfwSetKeyCallback(window, keyCallback);
  if (!capturePath) { // keep captures deterministic: no mouse-look
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);            // capture the mouse cursor
  }

  if (!gladLoadGL()) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  // On hybrid-graphics laptops this tells you which GPU you actually got
  std::cout << "GPU: " << glGetString(GL_RENDERER) << " (OpenGL "
            << glGetString(GL_VERSION) << ")" << std::endl;
  enableGLDebugOutput();
  glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "Maximum number of vertex attributes supported: " << nrAttributes << std::endl;

  // set of vertices
  float vertices[] = {
      // positions          // normals           // texture coords
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // texture coordinate attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // set up light VAO
  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  Texture diffuseMap = Texture();
  Texture specularMap = Texture();
  diffuseMap.loadTexture(containerPath);
  specularMap.loadTexture(containerSpecularPath);
  if (diffuseMap.id == 0 || specularMap.id == 0)
    return 1;

  configureLitShader(shader);

  Model backpack(backpackPath);
  glm::vec3 backpackPos(0.0f, 0.0f, 0.0f); // takes crate 0's spot

  // Bind something to every unit the lit shader samples before the first
  // frame; otherwise NVIDIA warns that unit 1 is empty when glClear runs.
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, diffuseMap.id);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, specularMap.id);

  float prevTime = glfwGetTime();
  float deltaTime = 0.0f;
  float angle = 0.0f;
  float rotationSpeed = glm::radians(180.0f); // 90 degrees per second
  float radius = 10.0f;

  glEnable(GL_DEPTH_TEST); // enable depth testing

  while (!glfwWindowShouldClose(window)) {

    float now = glfwGetTime(); // read once so no time is lost between calls
    deltaTime = now - prevTime;
    prevTime = now;

    // Hot reload: rebuild when a shader file is saved (or R is pressed).
    // A new program has no uniforms set, so re-apply the static ones.
    bool forceReload = reloadRequested;
    reloadRequested = false;
    if (forceReload ? shader.reload() : shader.reloadIfChanged())
      configureLitShader(shader);
    if (forceReload)
      lightShader.reload();
    else
      lightShader.reloadIfChanged();

    float cameraAngle = (float)glfwGetTime() * rotationSpeed;

    glm::mat4 view = camera.calculateLookAt();
    glm::mat4 perspective = glm::perspective(
        glm::radians(camera.getZoom()),
        (float)framebufferWidth / (float)std::max(framebufferHeight, 1), 0.1f,
        100.0f);

    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", perspective);

    // check if the escape key was pressed or the window was closed
    processInput(window, deltaTime);

    // rendering commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::vec3 lightPos =
        glm::vec3(radius * cos(cameraAngle), 1.0f, radius * sin(cameraAngle));
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);

    lightShader.use();
    lightShader.setMat4("model", model);
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", perspective);
    glBindVertexArray(lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    shader.use();
    shader.setVec3("viewPos", camera.CameraPos);
    shader.setVec3("spotLight.position", camera.CameraPos);
    shader.setVec3("spotLight.direction", camera.CameraFront);
    shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader.setVec3("pointLight.position", lightPos);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap.id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap.id);
    glBindVertexArray(VAO);
    for (unsigned int i = 1; i < 10; i++) { // crate 0 replaced by backpack
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePositions[i]);
      shader.setMat4("model", model);
      shader.setMat3("normalMatrix",
                     glm::mat3(glm::transpose(glm::inverse(model))));
      glDrawArrays(GL_TRIANGLES, 0, 36); // update buffers
    }

    // the backpack: Mesh::Draw binds its own diffuse/specular maps
    glm::mat4 backpackModel = glm::translate(glm::mat4(1.0f), backpackPos);
    backpackModel = glm::rotate(backpackModel, now * 0.4f, glm::vec3(0.0f, 1.0f, 0.0f));
    backpackModel = glm::scale(backpackModel, glm::vec3(0.45f));
    shader.setMat4("model", backpackModel);
    shader.setMat3("normalMatrix",
                   glm::mat3(glm::transpose(glm::inverse(backpackModel))));
    backpack.Draw(shader);

    static int frameCount = 0;
    if (capturePath && ++frameCount == captureFrame) {
      saveScreenshot(capturePath, framebufferWidth, framebufferHeight);
      glfwSetWindowShouldClose(window, true);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // cleanup
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  glDeleteVertexArrays(1, &lightVAO);
  glDeleteTextures(1, &diffuseMap.id);
  glDeleteTextures(1, &specularMap.id);

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
  framebufferWidth = width;
  framebufferHeight = height;
  glViewport(0, 0, width, height);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                 int mods) {
  if (key == GLFW_KEY_R && action == GLFW_PRESS)
    reloadRequested = true;
}

// Uniforms that never change per frame. Lives in a function because a
// hot-reloaded shader is a brand new program with every uniform reset to 0.
void configureLitShader(Shader &shader) {
  shader.use();

  // set material properties
  shader.setInt("material.diffuse", 0);
  shader.setInt("material.specular", 1);
  shader.setFloat("material.shininess", 32.0f);

  // set light properties
  shader.setVec3("pointLight.ambient", glm::vec3(0.02f, 0.02f, 0.02f));
  shader.setVec3("pointLight.diffuse", glm::vec3(0.6f, 0.6f, 0.6f));
  shader.setVec3("pointLight.specular", glm::vec3(0.2f, 0.2f, 0.2f));
  shader.setFloat("pointLight.constant", 1.0f);
  shader.setFloat("pointLight.linear", 0.027f);
  shader.setFloat("pointLight.quadratic", 0.0028f);

  // set directional light properties
  shader.setVec3("dirLight.direction", glm::vec3(-0.5f, -0.5f, -0.5f));
  shader.setVec3("dirLight.ambient", glm::vec3(0.01f, 0.01f, 0.01f));
  shader.setVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
  shader.setVec3("dirLight.specular", glm::vec3(0.05f, 0.05f, 0.05f));

  // set spot light properties
  shader.setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
  shader.setVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
  shader.setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));
  shader.setFloat("spotLight.constant", 1.0f);
  shader.setFloat("spotLight.linear", 0.027f);
  shader.setFloat("spotLight.quadratic", 0.0028f);
}

// Write the back buffer to a binary PPM (viewable in any image viewer).
bool saveScreenshot(const char *path, int width, int height) {
  std::vector<unsigned char> pixels(width * height * 3);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  glReadBuffer(GL_BACK);
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
  FILE *f = std::fopen(path, "wb");
  if (!f)
    return false;
  std::fprintf(f, "P6\n%d %d\n255\n", width, height);
  for (int y = height - 1; y >= 0; y--) // GL rows start at the bottom
    std::fwrite(&pixels[y * width * 3], 1, width * 3, f);
  std::fclose(f);
  std::cout << "Saved screenshot to " << path << std::endl;
  return true;
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
  camera.mouseCallback(window, xpos, ypos);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  camera.scrollCallback(window, xoffset, yoffset);
}
