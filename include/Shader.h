#ifndef __SHADER_H__
#define __SHADER_H__

#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdint.h>
#include <string>

class Shader {
public:
  // program ID
  unsigned int ID;

  // @brief Construct a shader program from vertex and fragment shader source
  // files
  // @param vertex_path   Path to the vertex shader source file
  // @param fragment_path Path to the fragment shader source file
  Shader(const char *vertex_path, const char *fragment_path);

  // @brief Activate the shader
  void use();

  // @brief Delete the shader
  void deleteShader();

  // @brief Recompile the program from its source files.
  //        On failure the previous (working) program is kept and the error is
  //        printed, so a typo in a shader never leaves you with a black screen.
  //        NOTE: uniforms belong to a program object, so a new program starts
  //        with every uniform reset to 0 - re-set them after a reload.
  // @return true if a new program was built and swapped in
  bool reload();

  // @brief Reload if either source file was saved since the last build.
  // @return true if a new program was swapped in (re-set your uniforms!)
  bool reloadIfChanged();

  // @brief Set a boolean uniform in the shader
  // @param name  Name of the uniform variable
  // @param value Boolean value to set
  void setBool(const std::string &name, bool value) const;

  // @brief Set an integer uniform in the shader
  // @param name  Name of the uniform variable
  // @param value Integer value to set
  void setInt(const std::string &name, int value) const;

  // @brief Set a float uniform in the shader
  // @param name  Name of the uniform variable
  // @param value Float value to set
  void setFloat(const std::string &name, float value) const;

  // @brief Set a 4x4 matrix uniform in the shader
  // @param name  Name of the uniform variable
  // @param value Reference to a glm::mat4 representing the matrix
  void setMat4(const std::string &name, const glm::mat4 value) const;

  // @brief Set a 3x3 matrix uniform in the shader
  // @param name  Name of the uniform variable
  // @param value Reference to a glm::mat3 representing the matrix
  void setMat3(const std::string &name, const glm::mat3 value) const;

  // @brief Set a 3-component vector uniform in the shader
  // @param name Name of the uniform variable
  // @param value Reference to a glm::vec3 representing the vector
  void setVec3(const std::string &name, const glm::vec3 value) const;

private:
  std::string _vertexPath;
  std::string _fragmentPath;
  std::filesystem::file_time_type _vertexTime;
  std::filesystem::file_time_type _fragmentTime;

  // @brief Read, compile and link both stages.
  // @return the new program ID, or 0 on any failure (errors are printed)
  unsigned int build() const;

  // @brief Record the current modification times of both source files
  void stampFileTimes();
};
#endif // __SHADER_H__
