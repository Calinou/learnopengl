#version 330 core

in vec3 fragPos;
in vec3 normal;

out vec4 fragColor;

uniform vec3 viewPos;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float glossiness;
};

uniform Material material;

struct Light {
  vec3 position;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform Light light;

void main() {
  // Ambient lighting
  vec3 ambient = light.ambient * material.ambient;

  // Diffuse lighting
  vec3 norm = normalize(normal);
  vec3 lightDir = normalize(light.position - fragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = light.diffuse * material.diffuse * diff;

  // Specular lighting
  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflectDir = reflect(-lightDir, norm);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.glossiness);
  vec3 specular = light.specular * material.specular * spec;

  vec3 result = ambient + diffuse + specular;

  fragColor = vec4(result, 1.0);
}
