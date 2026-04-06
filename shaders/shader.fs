#version 330 core

// Fragment shader for lighting calculations
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct PointLight {
	vec3 position;
		
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	
	float constant;
	float linear;
	float quadratic;
};

struct DirectionalLight {
	vec3 direction;
		
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	
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

// light properies
uniform PointLight pointLight;
uniform DirectionalLight dirLight;
uniform SpotLight spotLight;

uniform vec3 viewPos;

// Material properties
uniform Material material;

vec3 CalcDirLight(DirectionalLight light);
vec3 CalcPointLight(PointLight light);
vec3 CalcSpotLight(SpotLight light);
void main() {

	vec3 spotLightResult = CalcSpotLight(spotLight);
	vec3 dirLightResult = CalcDirLight(dirLight);
	vec3 pointLightResult = CalcPointLight(pointLight);

	vec3 result = spotLightResult + dirLightResult + pointLightResult;
	FragColor = vec4(result, 1.0); 																		// set all 4 vector values to 1.0
}

vec3 CalcPointLight(PointLight light) {
	vec3 lightDir = normalize(light.position - FragPos);
	// diffuse shading
	float diff = max(dot(Normal, lightDir), 0.0);
	// specular shading
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	return (ambient + diffuse + specular);
}

vec3 CalcDirLight(DirectionalLight light) {
	vec3 lightDir = normalize(-light.direction);
	// diffuse shading
	float diff = max(dot(Normal, lightDir), 0.0);
	// specular shading
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light) {
	vec3 lightDir = normalize(light.position - FragPos);
	// diffuse shading
	float diff = max(dot(Normal, lightDir), 0.0);
	// specular shading
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, Normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
	// spotlight intensity
	float theta = dot(lightDir, normalize(-light.direction)); 
	float epsilon = light.cutOff - 0.01;
	float intensity = clamp((theta - epsilon) / (light.cutOff - epsilon), 0.0, 1.0);
	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	return (ambient + diffuse + specular) * attenuation * intensity;
}