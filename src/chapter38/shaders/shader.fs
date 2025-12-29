#version 330 core

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    sampler2D texture_normal1;
    sampler2D texture_depth1;
};

struct PointLight{
    vec3 color;

    float constant;
    float linear;
    float quadratic;
};

vec3 CalcPointLight();
vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);


in vec2 TexCoords;
in vec3 FragPos;
in vec3 tLightPos;
in vec3 tViewPos;
in vec3 tFragPos;

out vec4 FragColor;

uniform Material material;
uniform PointLight light;
uniform float height_scale;


void main()
{
    vec3 result = vec3(0.0f);

    result += CalcPointLight();

    FragColor = vec4( result, 1.0f);
}

vec3 CalcPointLight()
{
    vec3 lightDir = normalize(tLightPos - tFragPos);
    vec3 viewDir = normalize(tViewPos - tFragPos);
    vec3 halfway = normalize(lightDir + viewDir);
    vec2 texCoords = ParallaxMapping(TexCoords, viewDir);
    if(texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    vec3 normal = texture(material.texture_normal1, texCoords).rgb;
    normal = normalize(normal * 2.0f - 1.0f);

    float dist = length(tLightPos - tFragPos);
    
    float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist);

     // Optional: Add a small ambient light so objects are never fully black
    vec3 ambient = 0.05 * vec3(texture(material.texture_diffuse1, texCoords));

    // diffuse light
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = vec3(texture(material.texture_diffuse1, texCoords)) * diff * light.color * attenuation;

    // specular light
    float spec = pow(max(dot(normal, halfway), 0.0), 64);
    vec3 specular = texture(material.texture_specular1, texCoords).rgb * spec * light.color * attenuation;

    return  (diffuse + specular + ambient);
}

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{
    float numLayers = 10;
    float layerDepth = 1.0 / numLayers;
    float currentLayerDepth = 0.0;
    vec2 p = viewDir.xy * height_scale;
    vec2 deltaTexCoords = p / numLayers;

    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = texture(material.texture_depth1, currentTexCoords).r;

    while(currentLayerDepth < currentDepthMapValue){
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(material.texture_depth1, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(material.texture_depth1, prevTexCoords).r
                        - currentLayerDepth + layerDepth;
    
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finaltexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finaltexCoords;
}