#version 330 core

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D textureBase;
uniform sampler2D textureOverlay;

void main() {
  // Take the overlay texture's alpha channel into account for blending textures
  fragColor =
      mix(
        texture(textureBase, texCoord),
        texture(textureOverlay, texCoord),
        vec4(texture(textureOverlay, texCoord)).a
      );
}
