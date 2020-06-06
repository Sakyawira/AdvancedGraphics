#version 450 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec4 FragPosLightSpace;

out vec4 color;

uniform float currentTime;
uniform sampler2D tex;

//uniform float ambientStr = 0.05f;
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);

out vec4 finalColor;

uniform vec3 objectColor;
uniform float specularStrength;
uniform float ambientStrength;
uniform vec3 cameraPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D texture_diffuse0;
uniform mat4 model;
uniform sampler2D shadowMap; 

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
    //// Ambient
    //vec3 ambient = ambientStr * ambientColor;

    //// Light Direction
    //vec3 norm = normalize(fragNormal);
    //vec3 lightDir = normalize(fragPos - lightPos);

    //// Diffuse Coloring
    //float diffuseStr = max(dot(norm, -lightDir), 0.0f);
    //vec3 diffuse = diffuseStr * lightColor;

    //color =  (texture(tex, fragTexCoord));// * vec4(ambient + diffuse, 1.0f);

   
    	
    //float intensity;
    //float toonStrength;
    //intensity = dot(lightDir,normalize(fragNormal));

    vec4 objectPos = model * vec4(1.0f);
    vec3 lightDir = normalize(fragPos - lightPos);
    // vec3 lightDir = objectPos.xyz - lightPos;

    // Ambient
    vec3 ambient = ambientStrength * ambientColor;

    // Light Direction
    vec3 norm = normalize(fragNormal);
   

    // Diffuse Coloring
    float diffuseStr = max(dot(norm, -lightDir), 0.0f);
    vec3 diffuse = diffuseStr * lightColor;
   
    		
    
    float shadow = ShadowCalculation(FragPosLightSpace);
    vec3 objectColor = ambient + ((shadow) * (diffuse));
    
    gl_FragColor = texture(tex, fragTexCoord) * vec4(objectColor, 1.0f);// * toonStrength;// * shadow;
}