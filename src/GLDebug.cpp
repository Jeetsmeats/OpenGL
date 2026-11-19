#include "GLDebug.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace {

const char *sourceName(GLenum source) {
  switch (source) {
  case GL_DEBUG_SOURCE_API: return "API";
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM: return "Window System";
  case GL_DEBUG_SOURCE_SHADER_COMPILER: return "Shader Compiler";
  case GL_DEBUG_SOURCE_THIRD_PARTY: return "Third Party";
  case GL_DEBUG_SOURCE_APPLICATION: return "Application";
  default: return "Other";
  }
}

const char *typeName(GLenum type) {
  switch (type) {
  case GL_DEBUG_TYPE_ERROR: return "ERROR";
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED";
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: return "UNDEFINED BEHAVIOUR";
  case GL_DEBUG_TYPE_PORTABILITY: return "PORTABILITY";
  case GL_DEBUG_TYPE_PERFORMANCE: return "PERFORMANCE";
  default: return "OTHER";
  }
}

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei /*length*/,
                            const GLchar *message, const void * /*user*/) {
  // Ignore chatty informational messages (e.g. NVIDIA's "Buffer object will
  // use VIDEO memory" notices) - they are not problems.
  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    return;
  // Shader.cpp already prints compile/link logs with the file name attached.
  if (source == GL_DEBUG_SOURCE_SHADER_COMPILER)
    return;

  std::cerr << "[GL " << typeName(type) << "] (" << sourceName(source)
            << ", id " << id << ") " << message << std::endl;
  // Tip: put a breakpoint on the line above. Because output is synchronous,
  // the call stack will point at the exact gl* call that caused it.
}

} // namespace

bool enableGLDebugOutput() {
  GLint flags = 0;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  // glad only loads glDebugMessageCallback for GL >= 4.3 contexts. In a 3.3
  // context (what NVIDIA gives you when you ask for 3.3) the same functions
  // come from the KHR_debug extension, so fetch them by hand.
  if (glad_glDebugMessageCallback == NULL &&
      glfwExtensionSupported("GL_KHR_debug")) {
    glad_glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)
        glfwGetProcAddress("glDebugMessageCallback");
    glad_glDebugMessageControl = (PFNGLDEBUGMESSAGECONTROLPROC)
        glfwGetProcAddress("glDebugMessageControl");
  }
  if (!(flags & GL_CONTEXT_FLAG_DEBUG_BIT) || glad_glDebugMessageCallback == NULL ||
      glad_glDebugMessageControl == NULL) {
    std::cout << "GL debug output unavailable (no debug context)" << std::endl;
    return false;
  }
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // report inside the failing call
  glDebugMessageCallback(debugCallback, nullptr);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr,
                        GL_TRUE);
  std::cout << "GL debug output enabled" << std::endl;
  return true;
}
