#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int32_t)value);
  };

  void setInt(const std::string &name, int32_t value) const {
    glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
  };

  void setFloat(const std::string &name, float_t value) const {
    glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
  };

  void setVec2(const std::string &name, glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
  }

  void setVec2(const std::string &name, float_t x, float_t y) const {
    glUniform2f(glGetUniformLocation(this->id, name.c_str()), x, y);
  }

  void setVec3(const std::string &name, glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
  }

  void setVec3(const std::string &name, float_t x, float_t y, float_t z) const {
    glUniform3f(glGetUniformLocation(this->id, name.c_str()), x, y, z);
  }

  void setVec4(const std::string &name, glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
  }

  void setVec4(const std::string &name, float_t x, float_t y, float_t z, float_t w) const {
    glUniform4f(glGetUniformLocation(this->id, name.c_str()), x, y, z, w);
  }

  void setMat2(const std::string &name, glm::mat2 &matrix) const {
    glUniformMatrix2fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
  }

  void setMat3(const std::string &name, glm::mat3 &matrix) const {
    glUniformMatrix3fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
  }

  void setMat4(const std::string &name, glm::mat4 &matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &matrix[0][0]);
  }
};
