#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

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

uniform Material material;
uniform Light light;

void main()
{
    // ambient lighting
    vec3 ambient = light.ambient * material.ambient;

    // calculate the fragment normal and directional vector to the light source
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    // light diffusion
    float diff = max(dot(norm, lightDir), 0.0);             // diffusion strength
    vec3 diffuse = light.diffuse * (diff * material.diffuse);                       // diffusion color
    
    // specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  

    // lighting result
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}