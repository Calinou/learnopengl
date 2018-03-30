#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "\n"
    "void main() {\n"
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n";

const char *fragmentShaderSource =
    "#version 330 core\n"
    "out vec4 fragColor;\n"
    "uniform vec4 ourColor;\n"
    "\n"
    "void main() {\n"
    "  fragColor = ourColor;\n"
    "}\n";

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int32_t main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(800, 600, "Shader Uniforms", nullptr, nullptr);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  if (window == nullptr) {
    std::cout << "Failed to create GLFW window." << std::endl;
    glfwTerminate();
    return -1;
  }

  // GLFW doesn't mark the context as current automatically
  // <https://stackoverflow.com/questions/48650497/glad-failing-to-initialize>
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD." << std::endl;
    return -1;
  }

  glViewport(0, 0, 800, 600);

  // Compile the vertex shader
  uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);

  int32_t success;
  char infoLog[512];

  // Check for successful vertex shader compilation
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cout << "Failed to compile the vertex shader.\n" << infoLog << std::endl;
  }

  // Compile the fragment shader
  uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);

  // Check for successful fragment shader compilation
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    std::cout << "Failed to compile the fragment shader.\n" << infoLog << std::endl;
  }

  // Create the shader program and attach shaders to it
  uint32_t shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  // Check for successful shader program compilation
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
  }

  // Link the shader program
  // It has to be used later since it is updated on every frame
  glLinkProgram(shaderProgram);

  // Delete shaders as they are no longer needed at this stage
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // The square's vertice coordinates
  float vertices[] = {
      -0.5f,  0.5f, 0.0f, // Top-left corner
      0.5f,  0.5f, 0.0f, // Top-right corner
      -0.5f, -0.5f, 0.0f, // Bottom-left corner
      0.5f, -0.5f, 0.0f, // Bottom-right corner
  };

  // The order to draw vertices in
  uint32_t indices[] = {
      0, 1, 2, // First triangle
      3, 2, 1, // Second triangle
  };

  // Initialize buffers (vertex array, vertex buffer, element buffer)
  uint32_t vao, vbo, ebo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);
  glGenBuffers(1, &ebo);

  // Bind buffers
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

  // Copy vertex data into the VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // Copy index data into the EBO
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Set vertex attribute parameters
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
  glEnableVertexAttribArray(0);
  // Fetch uniform location since we need to modify it in the render loop
  int32_t ourColor = glGetUniformLocation(shaderProgram, "ourColor");

  while (!glfwWindowShouldClose(window)) {
    processInput(window);
    double_t timeValue = glfwGetTime();
    double_t greenValue = sin(timeValue) / 2.0f + 0.5f;

    // Clear the viewport with a constant color
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the rectangle, setting a new color every frame
    glUseProgram(shaderProgram);
    glUniform4f(ourColor, 0.0f, (float_t) greenValue, 0.0f, 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Clean up
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glDeleteBuffers(1, &ebo);
  glfwTerminate();

  return 0;
}
