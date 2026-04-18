#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/gl.h>

#include <stdint.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:

    // program ID
    unsigned int ID;

    // @brief Construct a shader program from vertex and fragment shader source files
    // @param vertex_path   Path to the vertex shader source file
    // @param fragment_path Path to the fragment shader source file
    Shader(const char* vertex_path, const char* fragment_path);

    // @brief Activate the shader
    void use();

    // @brief Delete the shader
    void deleteShader();

    // @brief Set a boolean uniform in the shader
    // @param name  Name of the uniform variable
    // @param value Boolean value to set
    void setBool(const std::string& name, bool value) const;

    // @brief Set an integer uniform in the shader
    // @param name  Name of the uniform variable
    // @param value Integer value to set
    void setInt(const std::string& name, int value) const;

    // @brief Set a float uniform in the shader
    // @param name  Name of the uniform variable
    // @param value Float value to set
    void setFloat(const std::string& name, float value) const;

    // @brief Set a 4x4 matrix uniform in the shader
    // @param name  Name of the uniform variable
    // @param value Reference to a glm::mat4 representing the matrix
    void setMat4(const std::string& name, const glm::mat4 value) const;

    // @brief Set a 3x3 matrix uniform in the shader
    // @param name  Name of the uniform variable
    // @param value Reference to a glm::mat3 representing the matrix
    void setMat3(const std::string& name, const glm::mat3 value) const;

    // @brief Set a 3-component vector uniform in the shader
    // @param name Name of the uniform variable
    // @param value Reference to a glm::vec3 representing the vector
    void setVec3(const std::string& name, const glm::vec3 value) const;
};
#endif // __SHADER_H__