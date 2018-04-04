#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "Shader.hpp"

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

int32_t main() {
  constexpr int32_t screenWidth = 800;
  constexpr int32_t screenHeight = 800;

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "Camera", nullptr, nullptr);
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

  glViewport(0, 0, screenWidth, screenHeight);

  Shader shaderProgram = Shader(
      "../resources/shaders/camera.vertex.glsl",
      "../resources/shaders/camera.fragment.glsl"
  );

  // The cube's vertice coordinates
  float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  };

  // The cube positions
  glm::vec3 cubePositions[] = {
      glm::vec3(-2.0f, 0.0f, -3.0f),
      glm::vec3(2.0f, 0.0f, -3.0f),
      glm::vec3(2.0f, -2.0f, 3.0f),
      glm::vec3(2.0f, -2.5f, -3.0f),
      glm::vec3(-2.0f, 3.0f, -4.0f),
      glm::vec3(-1.0f, 1.0f, -5.0f),
      glm::vec3(1.0f, -1.0f, -6.0f),
      glm::vec3(-2.0f, 3.5f, -7.0f),
      glm::vec3(-2.0f, -1.0f, -10.0f),
      glm::vec3(0.0f, 0.0f, -1.0f),
  };

  stbi_set_flip_vertically_on_load(true);

  // Load image textures
  int32_t textureBaseWidth, textureBaseHeight, textureBaseChannels;
  unsigned char *textureBaseData = stbi_load(
      "../resources/textures/container.jpg",
      &textureBaseWidth,
      &textureBaseHeight,
      &textureBaseChannels,
      0
  );

  int32_t textureOverlayWidth, textureOverlayHeight, textureOverlayChannels;
  unsigned char *textureOverlayData = stbi_load(
      "../resources/textures/trixiestomp.png",
      &textureOverlayWidth,
      &textureOverlayHeight,
      &textureOverlayChannels,
      0
  );

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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) nullptr);
  glEnableVertexAttribArray(0);
  // Texture coordinates attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Generate the OpenGL textures
  uint32_t textureBase, textureOverlay;

  glGenTextures(1, &textureBase);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureBase);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glGenTextures(1, &textureBase);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureBase);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if (textureBaseData) {
    glTexImage2D(
        GL_TEXTURE_2D,     // Texture target
        0,                 // Mipmap level (0 = base resolution)
        GL_RGB,            // Color format
        textureBaseWidth,  // Width
        textureBaseHeight, // Height
        0,                 // Unused
        GL_RGB,            // Source image datatype,
        GL_UNSIGNED_BYTE,  // Source image RGB values are stored as `char`s (bytes)
        textureBaseData
    );
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "ERROR: Failed to load a texture." << std::endl;
  }

  glGenTextures(1, &textureOverlay);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textureOverlay);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Overlay texture has transparency, so it needs to be GL_RGBA
  if (textureOverlayData) {
    glTexImage2D(
        GL_TEXTURE_2D,        // Texture target
        0,                    // Mipmap level (0 = base resolution)
        GL_RGBA,              // Color format
        textureOverlayWidth,  // Width
        textureOverlayHeight, // Height
        0,                    // Unused
        GL_RGBA,              // Source image datatype,
        GL_UNSIGNED_BYTE,     // Source image RGB values are stored as `char`s (bytes)
        textureOverlayData
    );
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::cout << "ERROR: Failed to load a texture." << std::endl;
  }

  // Delete the texture data as it is not needed anymore at this point
  stbi_image_free(textureBaseData);
  stbi_image_free(textureOverlayData);

  shaderProgram.use();
  // Set the texture unit that should be sampled as the overlay texture
  shaderProgram.setInt("textureOverlay", 1);

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // Clear the viewport with a constant color
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Make the camera rotate in a circle around the center point
    float radius = 10.0f;
    double cameraX = sin(glfwGetTime()) * radius;
    double cameraZ = cos(glfwGetTime()) * radius;
    glm::vec3 cameraPosition = glm::vec3(cameraX, 0.0f, cameraZ);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    // Use an "up" vector to determine the camera's right axis using a cross product
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view;
    view = glm::lookAt(cameraPosition, cameraTarget, cameraUp);

    int32_t viewLoc = glGetUniformLocation(shaderProgram.id, "view");
    int32_t projectionLoc = glGetUniformLocation(shaderProgram.id, "projection");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Draw the 10 cubes
    // Model matrices are defined per-cube
    for (uint32_t i = 0; i < 10; i++) {
      glm::mat4 model;
      model = glm::translate(model, cubePositions[i]);
      float angle = 20.0f * i;
      model = glm::rotate(model, glm::radians(angle) + (float)glfwGetTime() * glm::radians(30.0f), glm::vec3(1.0f, 0.3f, 0.5f));

      int32_t modelLoc = glGetUniformLocation(shaderProgram.id, "model");
      // Draw the transformed scene
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // Clean up
  glDeleteVertexArrays(1, &vao);
  glDeleteBuffers(1, &vbo);
  glfwTerminate();

  return 0;
}
