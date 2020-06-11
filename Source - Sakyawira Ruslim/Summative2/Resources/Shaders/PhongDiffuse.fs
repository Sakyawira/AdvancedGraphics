#version 450 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec4 FragPosLightSpace;


out vec4 color;

uniform float currentTime;
uniform sampler2D tex;
uniform sampler2D shadowMap; 

uniform float ambientStr = 0.15f;
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);

uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos = vec3(-560.0f, 300.0f, -8.0f);

float ShadowCalculation(vec4 fragPosLightSpace)
{
   // To get NDC [-1, 1] from screenspace 
   	vec3 ndcSpace = FragPosLightSpace.xyz / FragPosLightSpace.w;
   
   	// Convert to Tex CoordSpace [0,1]
   	vec3 texCoordSpace = 0.5f * ndcSpace + 0.5f;
   
   	float bias = 0.000004f;
   	float currentDepth = texCoordSpace.z - bias;
  
   
   	float shadow;
   	vec2 texelSize = 1.0 / textureSize(shadowMap, 0); 
   	for (int x = -1; x <= 1; ++x) { 
   		for (int y = -1; y <= 1; ++y) { 
   			float pcfDepth = texture(shadowMap, texCoordSpace.xy + vec2(x, y) * texelSize).x; 
   			shadow += currentDepth< pcfDepth ? 1.0 : 0.3;
   		} 
   	}
   	shadow /= 9.0;
   	return shadow == 1.0 ? 1.0 : 0.3;
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