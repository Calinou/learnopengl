#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor; // The color will be passed to the fragment shader
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  // NOTE: The multiplication should be read from right to left
  gl_Position = projection * view * model * vec4(aPosition, 1.0f);
  ourColor = aColor;
  texCoord = aTexCoord;
}
