#include "Shader.h"
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>

namespace {

// Read a whole file into a string. Returns false if it can't be opened.
bool readFile(const std::string &path, std::string &out) {
  std::ifstream file(path);
  if (!file.is_open())
    return false;
  std::stringstream stream;
  stream << file.rdbuf();
  out = stream.str();
  return true;
}

// Compile one shader stage. Returns 0 on failure and prints the full log.
unsigned int compileStage(GLenum type, const std::string &source,
                          const std::string &path) {
  unsigned int shader = glCreateShader(type);
  const char *src = source.c_str();
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);

  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    // query the real log length instead of truncating at 512 chars
    int logLength = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    std::string log(logLength > 1 ? logLength : 1, '\0');
    glGetShaderInfoLog(shader, logLength, NULL, log.data());
    std::cout << "ERROR::SHADER::COMPILATION_FAILED in " << path << "\n"
              << log << std::endl;
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

} // namespace

Shader::Shader(const char *vertexPath, const char *fragmentPath)
    : ID(0), _vertexPath(vertexPath), _fragmentPath(fragmentPath) {
  stampFileTimes();
  ID = build();
}

unsigned int Shader::build() const {
  // 1. retrieve vertex/fragment source code from file paths
  std::string vertexCode, fragmentCode;
  if (!readFile(_vertexPath, vertexCode)) {
    std::cout << "ERROR::SHADER::FILE_NOT_READ " << _vertexPath << std::endl;
    return 0;
  }
  if (!readFile(_fragmentPath, fragmentCode)) {
    std::cout << "ERROR::SHADER::FILE_NOT_READ " << _fragmentPath << std::endl;
    return 0;
  }

  // 2. compile shaders
  unsigned int vertex = compileStage(GL_VERTEX_SHADER, vertexCode, _vertexPath);
  unsigned int fragment =
      compileStage(GL_FRAGMENT_SHADER, fragmentCode, _fragmentPath);
  if (vertex == 0 || fragment == 0) {
    glDeleteShader(vertex); // deleting 0 is a silent no-op
    glDeleteShader(fragment);
    return 0;
  }

  // 3. link program
  unsigned int program = glCreateProgram();
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  glDeleteShader(vertex);
  glDeleteShader(fragment);

  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    int logLength = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
    std::string log(logLength > 1 ? logLength : 1, '\0');
    glGetProgramInfoLog(program, logLength, NULL, log.data());
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED (" << _vertexPath
              << " + " << _fragmentPath << ")\n"
              << log << std::endl;
    glDeleteProgram(program);
    return 0;
  }
  return program;
}

void Shader::stampFileTimes() {
  std::error_code ec; // non-throwing overloads: a missing file is not fatal
  _vertexTime = std::filesystem::last_write_time(_vertexPath, ec);
  _fragmentTime = std::filesystem::last_write_time(_fragmentPath, ec);
}

bool Shader::reload() {
  stampFileTimes();
  unsigned int program = build();
  if (program == 0) {
    std::cout << "[shader] reload failed, keeping previous program"
              << std::endl;
    return false;
  }
  glDeleteProgram(ID);
  ID = program;
  std::cout << "[shader] reloaded " << _fragmentPath << std::endl;
  return true;
}

bool Shader::reloadIfChanged() {
  std::error_code ec;
  auto vt = std::filesystem::last_write_time(_vertexPath, ec);
  auto ft = std::filesystem::last_write_time(_fragmentPath, ec);
  if (ec || (vt == _vertexTime && ft == _fragmentTime))
    return false;
  return reload();
}

void Shader::use() { glUseProgram(ID); }

void Shader::deleteShader() { glDeleteProgram(ID); }

void Shader::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, const glm::mat4 value) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}

void Shader::setMat3(const std::string &name, const glm::mat3 value) const {
  glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
                     glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, const glm::vec3 value) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y,
              value.z);
}
