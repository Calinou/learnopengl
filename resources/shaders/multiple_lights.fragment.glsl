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

struct DirectionalLight {
  vec3 direction;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform DirectionalLight directionalLight;

struct PointLight {
  vec3 position;

  // Attenuation factors
  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

#define POINT_LIGHTS 4
uniform PointLight pointLights[POINT_LIGHTS];

struct SpotLight {
  vec3 position;
  vec3 direction;

  // Cutoff angles (stored as cosine values)
  float innerCutoff;
  float outerCutoff;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform SpotLight spotLight;

vec3 calcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(-light.direction);

  // Diffuse lighting
  float diff = max(dot(normal, lightDir), 0.0);

  // Specular lighting
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.glossiness);

  // Combine results
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
  vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

  return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  vec3 lightDir = normalize(light.position - fragPos);

  // Attenuation
  float distance = length(light.position - fragPos);
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2.0));

  // Diffuse lighting
  float diff = max(dot(normal, lightDir), 0.0);

  // Specular lighting
  vec3 reflectDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.glossiness);

  // Combine results
  vec3 ambient = light.ambient * attenuation * vec3(texture(material.diffuse, texCoords));
  vec3 diffuse = light.diffuse * attenuation * diff * vec3(texture(material.diffuse, texCoords));
  vec3 specular = light.specular * attenuation * spec * vec3(texture(material.specular, texCoords));

  return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
  // Ambient lighting
  vec3 ambient = light.ambient * texture(material.diffuse, texCoords).rgb;

  // Spot light computations
  vec3 lightDir = normalize(light.position - fragPos);
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.innerCutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);

  if (theta > light.outerCutoff) {
    // Diffuse lighting
    vec3 normal = normalize(normal);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * intensity * vec3(texture(material.diffuse, texCoords)) * diff;

    // Specular lighting
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.glossiness);
    vec3 specular = light.specular * intensity * vec3(texture(material.specular, texCoords)) * spec;

    return ambient + diffuse + specular;
  } else {
    return ambient;
  }
}

void main() {
  vec3 normal = normalize(normal);
  vec3 viewDir = normalize(viewPos - fragPos);

  // Directional light
  vec3 result = calcDirectionalLight(directionalLight, normal, viewDir);

  // Point lights
  for (int i = 0; i < POINT_LIGHTS; i++) {
    result += calcPointLight(pointLights[i], normal, fragPos, viewDir);
  }

  // Spot light
  result += calcSpotLight(spotLight, normal, fragPos, viewDir);

  fragColor = vec4(result, 1.0);
}
