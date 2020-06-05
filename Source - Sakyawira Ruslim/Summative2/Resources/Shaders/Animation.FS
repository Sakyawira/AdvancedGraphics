#version 450 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;
in vec4 FragPosLightSpace;

out vec4 color;

uniform float currentTime;
uniform sampler2D tex;

uniform float ambientStr = 0.05f;
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

    vec4 objectPos = model * vec4(1.0f);
    vec3 lightDir = objectPos.xyz - lightPos;
    	
    	float intensity;
    	float toonStrength;
    	intensity = dot(lightDir,normalize(fragNormal));
    
    if (intensity > 0.95)
    {
        toonStrength = 1.0f;
    }	
    else if (intensity > 0.5)
    {
        toonStrength = 0.8f;
    }
    else if (intensity > 0.25)
    {
        toonStrength = 0.5f;
    }
    else
    {
        toonStrength = 0.2f;
    }
    		
    
    //float shadow = ShadowCalculation();
    
    gl_FragColor = texture(tex, fragTexCoord) * vec4(objectColor, 1.0f);// * toonStrength;// * shadow;
}