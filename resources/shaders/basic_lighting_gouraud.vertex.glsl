#version 330 core

// In Gouraud shading, the lighting stage is performed in the vertex shader
// instead of the fragment shader.

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 vertColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main() {
  vec3 vertPos = vec3(model * vec4(aPos, 1.0));
  vec3 normal = mat3(transpose(inverse(model))) * aNormal;

  // Ambient lighting
  float ambientStrength = 0.1;
  vec3 ambient = ambientStrength * lightColor;

  // Diffuse lighting
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(lightPos - vertPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * lightColor;

  // Specular lighting
  float specularStrength = 0.5;
  vec3 viewDir = normalize(viewPos - vertPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
  vec3 specular = specularStrength * spec * lightColor;

  vertColor = (ambient + diffuse + specular) * objectColor;

  gl_Position = projection * view * vec4(vertPos, 1.0);
}
