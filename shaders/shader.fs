#version 330 core

// Fragment shader for lighting calculations
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform Light light;
uniform vec3 viewPos;

// Material properties
uniform Material material;

void main() {

	// ambient lighting
	vec3 ambient = light.ambient * material.ambient; // set all 3 vector values to 1.0

	// diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos); // light position - fragment position
	float diffFactor = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diffFactor * light.diffuse * material.diffuse;

	// specular lighting
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = diffuse + ambient + specular;
	FragColor = vec4(result, 1.0); // set all 4 vector values to 1.0
}