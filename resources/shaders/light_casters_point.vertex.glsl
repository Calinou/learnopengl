#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  // Pass the fragment position, normal and texture coordinates to the fragment shader
  fragPos = vec3(model * vec4(aPos, 1.0));
  normal = mat3(transpose(inverse(model))) * aNormal;
  texCoords = aTexCoord;

  gl_Position = projection * view * vec4(fragPos, 1.0);
}
