#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Model.hpp"

// Stored globally so it can be modified in framebufferSizeCallback() and used in main()
glm::mat4 projection;

// The field of view
constexpr float FOV = 50.0f;

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  projection = glm::perspective(glm::radians(FOV), (float) width / (float) height, 0.1f, 100.0f);
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

  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Model Loading",
                                        nullptr,
                                        nullptr);
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
  glEnable(GL_DEPTH_TEST);

  Shader modelShader = Shader(
      "../resources/shaders/model_loading.vertex.glsl",
      "../resources/shaders/model_loading.fragment.glsl"
  );

  Model ourModel = Model("../resources/models/nanosuit/nanosuit.blend");

  // Set the projection matrix here so it's defined on application start too
  projection = glm::perspective(glm::radians(FOV), (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT,
                                0.1f, 100.0f);

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

    modelShader.use();
    modelShader.setMat4("projection", projection);
    modelShader.setMat4("view", view);

    // Render the loaded model
    glm::mat4 model;
    model = glm::translate(model, glm::vec3(0.0, -1.75f, 0.0f));
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    modelShader.setMat4("model", model);
    ourModel.draw(modelShader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Clean up
  glfwTerminate();

  return 0;
}
