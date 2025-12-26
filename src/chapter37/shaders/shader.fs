#version 330 core

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
};

struct PointLight{
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

vec3 CalcPointLight();


in vec2 TexCoords;
in vec3 FragPos;
in vec3 tLightPos;
in vec3 tViewPos;
in vec3 tFragPos;

out vec4 FragColor;

uniform Material material;
uniform PointLight light;


void main()
{
    vec3 result = vec3(0.0f);

    result += CalcPointLight();

    // Optional: Add a small ambient light so objects are never fully black
    vec3 ambient = 0.05 * vec3(texture(material.texture_diffuse1, TexCoords));
    result += ambient;

    FragColor = vec4( result, 1.0f);
}

vec3 CalcPointLight()
{
    vec3 lightDir = normalize(tLightPos - tFragPos);
    vec3 viewDir = normalize(tViewPos - tFragPos);
    vec3 halfway = normalize(lightDir + viewDir);
    vec3 normal = texture(material.texture_normal1, TexCoords).rgb;
    normal = normalize(normal * 2.0f - 1.0f);

    float dist = length(tLightPos - tFragPos);
    
    float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    // diffuse light
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(texture(material.texture_diffuse1, TexCoords)) * diff * light.color * attenuation;

    // specular light
    float spec = pow(max(dot(normal, halfway), 0.0), 64);
    vec3 specular = texture(material.texture_specular1, TexCoords).rgb * spec * light.color * attenuation;

    return  (diffuse + specular);
}