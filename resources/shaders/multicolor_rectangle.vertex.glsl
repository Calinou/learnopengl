#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec3 ourColor; // The color will be passed to the fragment shader

uniform float offset;

void main() {
  gl_Position = vec4(aPosition.x + offset, aPosition.y + offset, aPosition.z + offset, 1.0);
  ourColor = aColor;
}
