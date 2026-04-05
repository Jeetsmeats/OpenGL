#version 330 core

// Fragment shader for lighting calculations
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform Light light;
uniform vec3 viewPos;

// Material properties
uniform Material material;

void main() {

	// calculate the attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// calculate the light direction
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);	
	// vec3 lightDir = normalize(-light.direction);
	float diffFactor = max(dot(norm, lightDir), 0.0);

	// calculate the view direction and reflection direction for specular lighting
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// ambient lighting
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords)); 					

	// diffuse lighting
	vec3 diffuse = diffFactor * light.diffuse * vec3(texture(material.diffuse, TexCoords));;

	// specular lighting
	vec3 specular = light.specular * (spec * vec3(texture(material.specular, TexCoords)));

	vec3 result = (ambient + diffuse + specular) * attenuation;
	FragColor = vec4(result, 1.0); 																		// set all 4 vector values to 1.0
}