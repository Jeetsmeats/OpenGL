#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct DirLight {
    // specified light direction
    vec3 direction;

    // light effects
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
    vec3 position;              // camera position
    vec3 direction;             // camera direction

    // lighting effects
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // attenuation
    float constant;
    float linear;
    float quadratic;

    // spotlight
    float cutOff;
    float outerCutOff;
};

struct PointLight {
    // light position
    vec3 position;
    
    // light effects
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // attenuation
    float constant;
    float linear;
    float quadratic;
};

#define N_POINT_LIGHTS 4

// declarations
// camera position
uniform vec3 viewPos;

// material characteristics
uniform Material material;

// light uniforms
uniform DirLight dirLight;
uniform PointLight pointLights[N_POINT_LIGHTS];
uniform SpotLight spotLight;

// functions
vec3 calculateDirLight(DirLight light, vec3 norm, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 norm, vec3 viewDir);
vec3 calculatePointLight(PointLight light, vec3 norm, vec3 viewDir);

void main()
{   
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);

    vec3 result = calculateDirLight(dirLight, norm, viewDir);
    // for (int i = 0; i < N_POINT_LIGHTS; i++) {
    //     result += calculatePointLight(pointLights[i], norm, viewDir);
    // }
    result += calculateSpotLight(spotLight, norm, viewDir);
    FragColor = vec4(result, 1.0);
} 

vec3 calculateDirLight(DirLight light, vec3 norm, vec3 viewDir) {

    vec3 lightDir = normalize(-light.direction);
    vec3 reflectDir = reflect(-lightDir, norm);

    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

    // diffuse
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  

    return ambient + diffuse + specular;
}

vec3 calculateSpotLight(SpotLight light, vec3 norm, vec3 viewDir) {

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2.0));

    // light vectors
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    // spot softening
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);

    // specular and diffuse strength
    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    // diffuse
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    // specular
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  

    return attenuation * (ambient + (diffuse * intensity) + (specular * intensity));
}

vec3 calculatePointLight(PointLight light, vec3 norm, vec3 viewDir) {

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * pow(distance, 2.0));

    // light vectors
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  

    // specular and diffuse strength
    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    // diffuse
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  
    // specular
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));  

    return attenuation * (ambient + diffuse + specular);
}