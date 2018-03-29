#version 330 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aColor;

out vec3 ourColor; // The color will be passed to the fragment shader

void main() {
  gl_Position = vec4(aPosition.x, aPosition.y, aPosition.z, 1.0);
  ourColor = aColor;
}
