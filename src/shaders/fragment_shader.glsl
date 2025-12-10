#version 330 core

struct Light{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Light light;
uniform Material material;

out vec4 FragColor;

in vec3 normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 viewPos;

void main()
{
    //AMBIENT LIGHT
    vec3 ambient = material.ambient * light.ambient;

    //DIFUSE LIGHT
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * material.diffuse * light.diffuse;

    //SPECULAR LIGHT
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * light.specular;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}