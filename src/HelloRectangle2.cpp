#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "\n"
    "void main() {\n"
    "  gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n";

const char *fragmentShaderTopSource =
    "#version 330 core\n"
    "out vec4 fragColor;\n"
    "\n"
    "void main() {\n"
    "  fragColor = vec4(0.5f, 1.0f, 0.5f, 1.0f);\n"
    "}\n";

const char *fragmentShaderBottomSource =
    "#version 330 core\n"
    "out vec4 fragColor;\n"
    "\n"
    "void main() {\n"
    "  fragColor = vec4(0.5f, 0.75f, 1.0f, 1.0f);\n"
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

  GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Rectangle 2", nullptr, nullptr);
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

  // Compile fragment shaders
  uint32_t fragmentShaderTop = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderTop, 1, &fragmentShaderTopSource, nullptr);
  glCompileShader(fragmentShaderTop);

  uint32_t fragmentShaderBottom = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShaderBottom, 1, &fragmentShaderBottomSource, nullptr);
  glCompileShader(fragmentShaderBottom);

  // We skip error checking here for the sake of concision

  // Create shader programs and attach shaders to them
  uint32_t shaderProgramTop = glCreateProgram();
  glAttachShader(shaderProgramTop, vertexShader);
  glAttachShader(shaderProgramTop, fragmentShaderTop);
  glLinkProgram(shaderProgramTop);

  uint32_t shaderProgramBottom = glCreateProgram();
  glAttachShader(shaderProgramBottom, vertexShader);
  glAttachShader(shaderProgramBottom, fragmentShaderBottom);
  glLinkProgram(shaderProgramBottom);

  // Delete shaders as they are no longer needed at this stage
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShaderTop);
  glDeleteShader(fragmentShaderBottom);

  // The square's vertice coordinates
  float vertices[] = {
      -0.5f,  0.5f, 0.0f, // Top-left corner
       0.5f,  0.5f, 0.0f, // Top-right corner
      -0.5f, -0.5f, 0.0f, // Bottom-left corner
       0.5f, -0.5f, 0.0f, // Bottom-right corner
  };

  // The orders to draw vertices in
  uint32_t indices[][3] = {
      {0, 1, 2}, // First triangle (drawn in green)
      {3, 2, 1}, // Second triangle (drawn in blue)
  };

  // Initialize buffers (vertex array, vertex buffer, element buffer)
  uint32_t vao[2], vbo[2], ebo[2];
  // It is possible to generate several buffers at once
  glGenVertexArrays(2, vao);
  glGenBuffers(2, vbo);
  glGenBuffers(2, ebo);

  // Set up the "top" triangle
  glBindVertexArray(vao[0]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]), &indices[0][0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
  glEnableVertexAttribArray(0);
  // No need to unbind the VAO since we bind to another VAO the next few lines

  // Set up the "bottom" triangle
  glBindVertexArray(vao[1]);
  glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[1]), &indices[1][0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
  glEnableVertexAttribArray(0);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // Clear the viewport with a constant color
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the top triangle
    glUseProgram(shaderProgramTop);
    glBindVertexArray(vao[0]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

    // Draw the bottom triangle
    glUseProgram(shaderProgramBottom);
    glBindVertexArray(vao[1]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Clean up stuff
  glDeleteVertexArrays(2, vao);
  glDeleteBuffers(2, vbo);
  glDeleteBuffers(2, ebo);
  glfwTerminate();

  return 0;
}
