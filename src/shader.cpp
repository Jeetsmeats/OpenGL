#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    // retrieve source code from file path
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;

    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    // throw exception
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        stringstream vShaderStream, fShaderStream;          // stream data
        
        // read file buffers into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // close handlers
        vShaderFile.close();
        fShaderFile.close();

        // convert stream into strings
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (ifstream::failure e) {
        
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // set up the vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    
    // print compilation errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
             << infoLog << endl;
    }

    // set up the fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);

    // print compilation errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
             << infoLog << endl;
    }
    
    // set ID as program ID
    ID = glCreateProgram();

    // attach shader and link together
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    // print linking errors
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
             << infoLog << endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()  {
    glUseProgram(ID);
}

void Shader::deleteShader() {
    glDeleteShader(ID);
}
void Shader::setBool(const string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void Shader::setInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const string &name, glm::mat4 mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setVec3(const string &name, glm::vec3 vector) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(vector));
}