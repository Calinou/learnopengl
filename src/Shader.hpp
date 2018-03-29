#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glad/glad.h>

class Shader {
public:
  uint32_t id;

  Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // Ensure ifstream objects can throw exceptions
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
      vShaderFile.open(vertexPath);
      fShaderFile.open(fragmentPath);
      std::stringstream vShaderStream, fShaderStream;
      // Read file's buffer contents into streams
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      // Close file handles
      fShaderFile.close();
      vShaderFile.close();
      // Convert streams into strings
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure &e) {
      std::cout << "ERROR: Failed reading a shader file." << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    uint32_t vertexShader, fragmentShader;
    int32_t success;
    char infoLog[512];

    // Compile the vertex shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    glCompileShader(vertexShader);

    // Check for vertex shader compilation errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
      glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
      std::cout << "ERROR: Failed compiling a vertex shader.\n       " << infoLog << std::endl;
    }

    // Compile the fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentShader);

    // Check for fragment shader compilation errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
      glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
      std::cout << "ERROR: Failed compiling a fragment shader.\n       " << infoLog << std::endl;
    }

    // Create the shader program
    this->id = glCreateProgram();
    glAttachShader(id, vertexShader);
    glAttachShader(id, fragmentShader);
    glLinkProgram(this->id);

    // Check for shader program linking errors
    glGetProgramiv(this->id, GL_LINK_STATUS, &success);

    if (!success) {
      glGetProgramInfoLog(this->id, 512, nullptr, infoLog);
      std::cout << "ERROR: Failed linking a shader program.\n       " << infoLog << std::endl;
    }

    // Delete shaders as they are now linked and no longer needed
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
  }

  void use() {
    glUseProgram(this->id);
  };

  void setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int32_t) value);
  };

  void setInt(const std::string &name, int32_t value) const {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
  };

  void setFloat(const std::string &name, float_t value) const {
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
  };
};
