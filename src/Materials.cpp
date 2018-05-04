#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include "Shader.hpp"

// Stored globally so it can be modified in framebufferSizeCallback() and used in main()
glm::mat4 projection;

// The field of view
constexpr float FOV = 50.0f;

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  projection = glm::perspective(glm::radians(FOV), (float)width / (float)height, 0.1f, 100.0f);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int32_t main() {
  // The initial window size
  constexpr int32_t SCREEN_WIDTH = 1280;
  constexpr int32_t SCREEN_HEIGHT = 720;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Materials", nullptr, nullptr);
  glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

  if (window == nullptr) {
    std::cout << "Failed to create GLFW window." << std::endl;
    glfwTerminate();
    return -1;
  }

  // GLFW doesn't mark the context as current automatically
  // <https://stackoverflow.com/questions/48650497/glad-failing-to-initialize>
  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    std::cout << "ERROR: Failed to initialize GLAD." << std::endl;
    return -1;
  }

  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  Shader containerShader = Shader(
      "../resources/shaders/materials.vertex.glsl",
      "../resources/shaders/materials.fragment.glsl"
  );

  Shader lightShader = Shader(
      "../resources/shaders/basic_lighting.vertex.glsl",
      "../resources/shaders/light_colors_bright.fragment.glsl"
  );

  // The cube's vertice and normal coordinates
  float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
      -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
  };

  // The light's position
  glm::vec3 lightPos = glm::vec3(1.0f, 0.0f, 0.8f);

  glEnable(GL_DEPTH_TEST);

  // Initialize buffers (vertex array, vertex buffer, element buffer)
  uint32_t vao, vbo;
  glGenVertexArrays(1, &vao);
  glGenBuffers(1, &vbo);

  // Bind buffers
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);

  // Copy vertex data into the VBO
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Set vertex attribute parameters
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) nullptr);
  glEnableVertexAttribArray(0);
  // Normal attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Create the light VAO
  uint32_t lightVao;
  glGenVertexArrays(1, &lightVao);
  glBindVertexArray(lightVao);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) nullptr);
  glEnableVertexAttribArray(0);

  // Set the projection matrix here so it's defined on application start too
  projection = glm::perspective(glm::radians(FOV), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // Clear the viewport with a constant color
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Make the camera rotate in a circle around the center point
    float radius = 3.0f;
    double cameraX = sin(glfwGetTime() * 0.75f) * radius;
    double cameraZ = cos(glfwGetTime() * 0.75f) * radius;
    glm::vec3 cameraPosition = glm::vec3(cameraX, 0.0f, cameraZ);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    // Use an "up" vector to determine the camera's right axis using a cross product
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view;
    view = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

    glm::vec3 lightColor;
    lightColor.x = (float) (sin(glfwGetTime() * 2.0f));
    lightColor.y = (float) (sin(glfwGetTime() * 1.0f));
    lightColor.z = (float) (sin(glfwGetTime() * 0.5f));

    glm::vec3 ambientColor = lightColor * glm::vec3(0.5f);
    glm::vec3 diffuseColor = lightColor * glm::vec3(0.2f);

    // Draw the container cube
    glm::mat4 model;
    // The container cube's position (bobbing up and down)
    glm::vec3 containerPos = glm::vec3(0.0f, sin(glfwGetTime()) * 0.5f - 0.25f, 0.0f);
    model = glm::translate(model, containerPos);
    containerShader.use();
    containerShader.setMat4("model", model);
    containerShader.setMat4("view", view);
    containerShader.setMat4("projection", projection);
    containerShader.setVec3("viewPos", cameraPosition);

    // Set material properties
    containerShader.setVec3("material.ambient", 0.4f, 1.0f, 0.2f);
    containerShader.setVec3("material.diffuse", 0.4f, 1.0f, 0.2f);
    containerShader.setVec3("material.specular", 1.0f, 1.0f, 1.0f);
    containerShader.setFloat("material.glossiness", 24.0f);

    // Set light properties
    containerShader.setVec3("light.position", lightPos);
    containerShader.setVec3("light.ambient", ambientColor);
    containerShader.setVec3("light.diffuse", diffuseColor);
    containerShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    // Draw the light cube
    model = glm::mat4();
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.15f));
    lightShader.use();
    lightShader.setMat4("model", model);
    lightShader.setMat4("view", view);
    lightShader.setMat4("projection", projection);
    glBindVertexArray(lightVao);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Clean up
  glDeleteVertexArrays(1, &vao);
  glDeleteVertexArrays(1, &lightVao);
  glDeleteBuffers(1, &vbo);
  glfwTerminate();

  return 0;
}
