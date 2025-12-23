#version 330 core

struct Material{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct PointLight{
    vec3 Pos;
    vec3 color;

    float constant;
    float linear;
    float quadratic; // FIXED: Typo corrected
};

vec3 CalcPointLight();

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform Material material;
uniform PointLight light;
uniform vec3 cameraPos;

void main()
{
    vec3 result = vec3(0.0f);
    
    // Optional: Add a small ambient light so objects are never fully black
    vec3 ambient = 0.1 * vec3(texture(material.diffuse, TexCoords));
    result += ambient;

    result += CalcPointLight();

    FragColor = vec4( result, 1.0f);
}

vec3 CalcPointLight()
{
    vec3 lightDir = normalize(light.Pos - FragPos);
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 halfway = normalize(lightDir + viewDir);
    vec3 normal = normalize(Normal);

    float dist = length(light.Pos - FragPos);
    
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    // diffuse light
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diff; // Extract RGB
    diffuse *= light.color * attenuation;

    // specular light
    float spec = pow(max(dot(normal, halfway), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords).r) * spec; // Extract RGB
    specular *= light.color * attenuation;

    return diffuse + specular;
}