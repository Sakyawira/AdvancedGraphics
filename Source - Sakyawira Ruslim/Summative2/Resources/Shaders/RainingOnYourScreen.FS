#version 450 core
 
in vec2 fragTexCoord;

out vec4 color;

uniform float currentTime;
uniform sampler2D tex1;
uniform sampler2D tex;
uniform vec2 iResolution;

void main()
{
	vec2 uv = fragTexCoord.xy / iResolution.xy;

	float time = currentTime;

	vec3 raintex = texture(tex1,vec2(uv.x*2.0,uv.y*0.1+time*0.125)).rgb/8.0;

	vec2 where = (uv.xy-raintex.xy);
	vec3 texchur1 = texture(tex,vec2(where.x,where.y)).rgb;
	
	color = vec4(texchur1,1.0);
}