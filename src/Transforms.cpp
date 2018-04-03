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
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(800, 800, "Transforms", nullptr, nullptr);
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

  glViewport(0, 0, 800, 800);

  Shader shaderProgram = Shader(
      "../resources/shaders/transforms.vertex.glsl",
      "../resources/shaders/transforms.fragment.glsl"
  );

  // The square's vertice coordinates, colors and texture coordinates
  float vertices[] = {
      // Positions        // Colors       // Texture coordinates
      -0.5f, 0.5f, 0.0f, 1.0f, 0.3f, 0.3f, 0.0f, 1.0f,  // Top-left corner
      0.5f, 0.5f, 0.0f, 0.3f, 1.0f, 0.3f, 1.0f, 1.0f,   // Top-right corner
      -0.5f, -0.5f, 0.0f, 0.3f, 0.3f, 1.0f, 0.0f, 0.0f, // Bottom-left corner
      0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,  // Bottom-right corner
  };

  // The order to draw vertices in
  uint32_t indices[] = {
      0, 1, 2, // First triangle
      3, 2, 1, // Second triangle
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
  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) nullptr);
  glEnableVertexAttribArray(0);
  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // Texture coordinates attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Generate the OpenGL textures
  uint32_t textureBase, textureOverlay;

  glGenTextures(1, &textureBase);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textureBase);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
  } else {
    std::cout << "ERROR: Failed to load a texture." << std::endl;
  }

  glGenTextures(1, &textureOverlay);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, textureOverlay);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
  } else {
    std::cout << "ERROR: Failed to load a texture." << std::endl;
  }

  // Delete the texture data as it is not needed anymore at this point
  stbi_image_free(textureBaseData);
  stbi_image_free(textureOverlayData);

  shaderProgram.use();
  // Set the texture unit that should be sampled as the overlay texture
  shaderProgram.setInt("textureOverlay", 1);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // Clear the viewport with a constant color
    glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // The transformation matrices (has to be updated every frame here)
    glm::mat4 translation;
    translation = glm::translate(translation, glm::vec3(0.5f, -0.5f, 0.0f));
    translation = glm::rotate(translation, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

    glm::mat4 translation2;
    double scaleFactor = sin(glfwGetTime())/2.5 + 0.6f;
    translation2 = glm::translate(translation2, glm::vec3(-0.5f, 0.5f, 0.0f));
    translation2 = glm::scale(translation2, glm::vec3(scaleFactor, scaleFactor, 1.0f));

    // Retrieve the transform uniform location and set a value
    int32_t transformLoc = glGetUniformLocation(shaderProgram.id, "transform");

    // Draw the transformed containers
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(translation));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(translation2));
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
