/**
 * @Author: Gunjeet Singh
 * @Date:   2025-02-11 16:56:58
 * @Last Modified by:   Your name
 * @Last Modified time: 2025-02-12 09:59:44
 */
#include <stdint.h>

#ifndef __SHADER_H__
#define __SHADER_H__

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader {
public:
    // program id
    unsigned int ID;

    /// @brief Reads and builds shader
    /// @param vertexPath Vertex source path
    /// @param fragmentPath Fragment source path
    Shader(const char *vertexPath, const char *fragmentPath);

    /// @brief Activate the shader
    void use();

    /// @brief Delete the shader
    void deleteShader();

    /// @brief Utility uniform function for setting boolean value
    /// @param name uniform name
    /// @param value uniform value
    void setBool(const string &name, bool value) const;

    /// @brief Utility uniform function for setting integer value
    /// @param name uniform name
    /// @param value uniform value
    void setInt(const string &name, int value) const;

    /// @brief Utility uniform function for setting float value
    /// @param name uniform name
    /// @param value uniform value
    void setFloat(const string &name, float value) const;

    /// @brief Utility uniform function for setting matrix
    /// @param name uniform name
    /// @param mat uniform matrix val
    void setMat4(const string &name, glm::mat4 mat) const;
};

#endif /* __SHADER_H__ */