#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    // ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // calculate the fragment normal and directional vector to the light source
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // light diffusion
    float diff = max(dot(norm, lightDir), 0.0);             // diffusion strength
    vec3 diffuse = diff * lightColor;                       // diffusion color

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}