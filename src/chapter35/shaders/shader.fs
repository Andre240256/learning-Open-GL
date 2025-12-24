#version 330 core

struct Material{
    sampler2D diffuse;
};

struct PointLight{
    vec3 Pos;
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

float ShadowCalculation(vec4 FragPosLightSpace, vec3 normal, vec3 lightDir);
vec3 CalcPointLight();


in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

out vec4 FragColor;

uniform Material material;
uniform PointLight light;
uniform sampler2D shadowMap;
uniform vec3 cameraPos;

void main()
{
    vec3 result = vec3(0.0f);

    result += CalcPointLight();

    // // Optional: Add a small ambient light so objects are never fully black
    vec3 ambient = 0.05 * vec3(texture(material.diffuse, TexCoords));
    result += ambient;

    FragColor = vec4( result, 1.0f);
}

float ShadowCalculation(vec4 FragPosLightSpace, vec3 normal, vec3 lightDir)
{
    float shadow =0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    for(int i = -1; i <= 1; i++)
        for(int j = -1; j <=1; j++)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + 
                                    vec2(i, j) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }


    shadow /= 9;
    if(projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

vec3 CalcPointLight()
{
    vec3 lightDir = normalize(light.Pos - FragPos);
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 halfway = normalize(lightDir + viewDir);
    vec3 normal = normalize(Normal);

    float dist = length(light.Pos - FragPos);
    
    float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    // diffuse light
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diff * light.color * attenuation;

    // specular light
    float spec = pow(max(dot(normal, halfway), 0.0), 32);
    vec3 specular = vec3(0.3f) * spec * light.color * attenuation;

    //shadow
    float shadow = ShadowCalculation(FragPosLightSpace, normal, lightDir);

    return  (diffuse + specular) * (1.0f - shadow);
}