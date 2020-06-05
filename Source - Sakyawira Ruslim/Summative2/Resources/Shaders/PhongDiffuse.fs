#version 450 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec4 FragPosLightSpace;


out vec4 color;

uniform float currentTime;
uniform sampler2D tex;
uniform sampler2D tex1;
uniform sampler2D shadowMap; 

uniform float ambientStr = 0.05f;
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);

uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos = vec3(-2.0f, 6.0f, 3.0f);

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // To get NDC [-1, 1] from screenspace 
    vec3 ndcSpace = fragPosLightSpace.xyz/fragPosLightSpace.w;
    // Convert to Tex Coord Space [0,1] 
    vec3 texCoordSpace =  0.5f * ndcSpace + 0.5f;
    float currentDepth = texCoordSpace.z ;
    Float closestDepth = texture(shadowMap, texCoordSpace.xy).r;
    float shadow = closestDepth > currentDepth ? 0.0 : 1.0;
    return shadow 
}

void main ()
{
    // Ambient
    vec3 ambient = ambientStr * ambientColor;

    // Light Direction
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(fragPos - lightPos);

    // Diffuse Coloring
    float diffuseStr = max(dot(norm, -lightDir), 0.0f);
    vec3 diffuse = diffuseStr * lightColor;

    // old
    // color =  (texture(tex, fragTexCoord)) * vec4(ambient + diffuse, 1.0f);

    // new
    float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 totalColor = ambient + ((shadow) * (diffuse));
    color = vec4(totalColor, 1.0f) * texture(tex, fragTexCoord) ;
}