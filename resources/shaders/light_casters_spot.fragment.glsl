#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;

uniform vec3 viewPos;

struct Material {
  sampler2D diffuse;
  sampler2D specular;
  float glossiness;
};

uniform Material material;

struct Light {
  vec3 position;
  vec3 direction;
  float cutoff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  // Constant attenuation factor (should generally be set to 1)
  float constant;
  // Linear attenuation factor
  float linear;
  // Quadratic attenuation factor
  float quadratic;
};

uniform Light light;

void main() {
  // Ambient lighting
  vec3 ambient = light.ambient * texture(material.diffuse, texCoords).rgb;

  // Spot light computations
  vec3 lightDir = normalize(light.position - fragPos);
  float theta = dot(lightDir, normalize(-light.direction));

  if (theta > light.cutoff) {
    // Diffuse lighting
    vec3 norm = normalize(normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * texture(material.diffuse, texCoords).rgb * diff;

    // Specular lighting
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.glossiness);
    vec3 specular = light.specular * texture(material.specular, texCoords).rgb * spec;

    fragColor = vec4(ambient + diffuse + specular, 1.0);
  } else {
    fragColor = vec4(ambient, 1.0);
  }
}
