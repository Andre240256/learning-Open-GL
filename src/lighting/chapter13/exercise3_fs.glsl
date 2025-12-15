#version 330 core
out vec4 FragColor;

in vec3 normal;
in vec3 FragPos;
in mat4 viewMat;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    //AMBIENT LIGHT
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    //DIFUSE LIGHT
    vec3 norm = normalize(normal);
    vec3 viewLightPos = vec3(viewMat * vec4(lightPos, 1.0f));
    vec3 lightDir = normalize( viewLightPos- FragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    //SPECULAR LIGHT
    float specularStrenght = 0.5f;
    vec3 viewDir = normalize( - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrenght * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}