#version 330 core

in vec2 texCoords;

out vec4 _fragColor;

uniform sampler2D textureDiffuse1;

void main() {
  _fragColor = texture(textureDiffuse1, texCoords);
}
