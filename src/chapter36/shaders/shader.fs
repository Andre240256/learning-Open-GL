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

out vec4 FragColor;

uniform Material material;
uniform PointLight light;
uniform samplerCube depthMap;
uniform vec3 cameraPos;
uniform float far_plane;

vec3 sampleOffsetDirections[20] = vec3[]
(
vec3( 1, 1, 1), vec3( 1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
vec3( 1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
vec3( 1, 1, 0), vec3( 1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
vec3( 1, 0, 1), vec3(-1, 0, 1), vec3( 1, 0, -1), vec3(-1, 0, -1),
vec3( 0, 1, 1), vec3( 0, -1, 1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

void main()
{
    vec3 result = vec3(0.0f);

    result += CalcPointLight();

    // // Optional: Add a small ambient light so objects are never fully black
    vec3 ambient = 0.05 * vec3(texture(material.diffuse, TexCoords));
    result += ambient;

    FragColor = vec4( result, 1.0f);
}

float ShadowCalculation(vec3 normal, vec3 lightDir)
{
    float shadow = 0.0;
    float bias = 0.005;
    int samples = 20;
    float viewDist = length(FragPos - cameraPos);
    float diskRadius = (1.0 + (viewDist / far_plane)) / 25.0f;
    vec3 fragToLight = FragPos - light.Pos;
    float currentDepth = length(fragToLight);
    for(int i  =0; i < samples; ++i)
    {
        float closestDepth = texture(depthMap, fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= far_plane;
        if(currentDepth - bias > closestDepth)
            shadow += 1.0f;
    }
    
    
    shadow /= float(samples);

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
    float spec = pow(max(dot(normal, halfway), 0.0), 64);
    vec3 specular = vec3(0.3f) * spec * light.color * attenuation;

    //shadow
    float shadow = ShadowCalculation(normal, lightDir);

    return  (diffuse + specular) * (1.0f - shadow);
}