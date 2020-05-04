#version 450 core 

in vec3 outColor; 

in vec2 TexCoords;

uniform sampler2D tex;

out vec4 color;
void main()
{
    color = vec4(outColor, 1.0f) * texture(tex, fragTexCoord);
}