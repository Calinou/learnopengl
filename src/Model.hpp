#pragma once

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "Shader.hpp"
#include "Mesh.hpp"

/**
 * Loads a texture from a file.
 *
 * @param path The texture file name
 * @param directory Path to the directory containing the texture file
 * @param gamma Whether to perform gamma correction on the loaded texture (TODO)
 * @return The assigned OpenGL texture ID
 */
uint32_t textureFromFile(const char *path, const std::string &directory, bool gamma = false) {
  std::string filename = std::string(path);
  filename = directory + '/' + filename;

  uint32_t textureId;
  glGenTextures(1, &textureId);

  int32_t width, height, nbComponents;
  uint8_t *data = stbi_load(filename.c_str(), &width, &height, &nbComponents, 0);

  if (data) {
    GLenum format;

    switch (nbComponents) {
      case 1:
        format = GL_RED;
        break;
      case 3:
        format = GL_RGB;
        break;
      case 4:
        format = GL_RGBA;
        break;
      default:
        std::cout
            << "WARNING: Attempted to load a texture with an unsupported number of components ("
            << nbComponents << "), falling back to RGB." << std::endl;
        format = GL_RGB;
        break;
    }

    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  } else {
    std::cout << "ERROR: Failed to load texture at path: " << path << std::endl;
  }

  // The texture resource is no longer needed at this point
  stbi_image_free(data);

  return textureId;
}

class Model {
public:
  explicit Model(const char *path) {
    loadModel(path);
  }

  void draw(Shader shader) {
    for (Mesh mesh : meshes) {
      mesh.draw(shader);
    }
  }

private:
  std::vector<Mesh> meshes;
  std::string directory;
  // Used for caching
  std::vector<Texture> textures_loaded;

  void loadModel(const std::string &path) {
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
      return;
    }

    this->directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene);
  }

  void processNode(aiNode *node, const aiScene *scene) {
    // Process all the node's meshes (if any)
    for (uint32_t i = 0; i < node->mNumMeshes; i++) {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      meshes.push_back(processMesh(mesh, scene));
    }

    // Do the same for each of its children
    for (uint32_t i = 0; i < node->mNumChildren; i++) {
      processNode(node->mChildren[i], scene);
    }
  }

  Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::vector<Texture> textures;

    // Process vertex positions, normal and texture coordinates
    for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
      Vertex vertex;
      glm::vec3 vector;

      vector.x = mesh->mVertices[i].x;
      vector.y = mesh->mVertices[i].y;
      vector.z = mesh->mVertices[i].z;
      vertex.position = vector;

      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.normal = vector;

      if (mesh->mTextureCoords[0]) {
        // Only use the first set of texture coordinates
        glm::vec2 vector2;
        vector2.x = mesh->mTextureCoords[0][i].x;
        vector2.y = mesh->mTextureCoords[0][i].y;
        vertex.texCoords = vector2;
      } else {
        vertex.texCoords = glm::vec2(0.0f, 0.0f);
      }

      vertices.push_back(vertex);
    }

    // Process indices
    for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];

      for (uint32_t j = 0; j < face.mNumIndices; j++) {
        indices.push_back(face.mIndices[j]);
      }
    }

    // Process the material
    if (mesh->mMaterialIndex >= 0) {
      aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

      std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE,
                                                              "texture_diffuse");
      textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

      std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR,
                                                               "texture_specular");
      textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    return Mesh(vertices, indices, textures);
  }

  std::vector<Texture>
  loadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName) {
    std::vector<Texture> textures;

    for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
      aiString str;
      mat->GetTexture(type, i, &str);

      // Check for already-loaded textures
      bool skip = false;
      for (Texture texture : this->textures_loaded) {
        if (std::strcmp(texture.path.data(), str.C_Str()) == 0) {
          textures.push_back(texture);
          skip = true;
          break;
        }
      }

      if (!skip) {
        // Load texture and cache it if it hasn't been loaded already
        Texture texture;
        texture.id = textureFromFile(str.C_Str(), this->directory);
        texture.type = typeName;
        texture.path = str.C_Str();
        this->textures_loaded.push_back(texture);
      }
    }

    return textures;
  }
};
