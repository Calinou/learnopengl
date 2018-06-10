#pragma once

#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Shader.hpp"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

struct Texture {
  uint32_t id;
  std::string type;
  // Used to compare with other textures (caching)
  std::string path;
};

class Mesh {
public:
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indices;
  std::vector<Texture> textures;

  Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures) {
    this->vertices = std::move(vertices);
    this->indices = std::move(indices);
    this->textures = std::move(textures);

    setupMesh();
  }

  void draw(Shader shader) {
    uint32_t diffuseNb = 1;
    uint32_t specularNb = 1;

    for (uint32_t i = 0; i < textures.size(); i++) {
      glActiveTexture(GL_TEXTURE0 + i);
      // Retrieve texture number
      std::string number;
      std::string name = textures[i].type;

      if (name == "texture_diffuse") {
        number = std::to_string(diffuseNb++);
      } else if (name == "texture_specular") {
        number = std::to_string(specularNb++);
      }

      shader.setFloat("material." + name + number, i);
      glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glActiveTexture(GL_TEXTURE0);

    // Draw the mesh
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, (GLsizei) indices.size(), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
  }

private:
  uint32_t vao, vbo, ebo;

  void setupMesh() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(2, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0],
                 GL_STATIC_DRAW);

    // Vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) offsetof(Vertex, position));

    // Vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) offsetof(Vertex, normal));

    // Vertex texture coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *) offsetof(Vertex, texCoords));

    glBindVertexArray(0);
  }
};
