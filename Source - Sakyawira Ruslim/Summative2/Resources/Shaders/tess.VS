//#version 450 core

//layout (location = 0) in vec3 position; 
//layout (location = 1) in vec3 normal;
//layout (location = 2) in vec2 texCoord;

//out vec4 mWorldPos;

//void main()
//{
//	gl_Position = vec4(position, 1.0f);
//}

#version 450 core 

layout (location = 0) in vec3 position; 
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoord;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoord;
out vec4 FragPosLightSpace;

uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 lightVPMatrix;

uniform mat4 MVP;

out vec4 mWorldPos;

void main ()
{
	gl_Position = vec4(position, 1.0f);
	fragPos = vec3(model * vec4(position, 1.0f));
	fragNormal =  mat3(transpose(inverse(model))) * normal;
	fragTexCoord = texCoord;
	FragPosLightSpace = lightVPMatrix * vec4(fragPos, 1.0f);
}