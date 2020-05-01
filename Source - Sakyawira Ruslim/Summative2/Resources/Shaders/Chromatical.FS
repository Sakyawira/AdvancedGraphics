#version 450 core
 
in vec2 fragTexCoord;

out vec4 color;

uniform float currentTime;
uniform sampler2D tex1;
uniform sampler2D tex;
uniform vec2 iResolution;

void main()
{
// distance from center of image, used to adjust blur
	vec2 uv = fragTexCoord.xy / iResolution.xy;
	float d = length(uv - vec2(0.5,0.5));
	
	// blur amount
	float blur = 0.0;	
	blur = (1.0 + sin(currentTime*6.0)) * 0.5;
	blur *= 1.0 + sin(currentTime*16.0) * 0.5;
	blur = pow(blur, 3.0);
	blur *= 0.05;
	// reduce blur towards center
	blur *= d;
	
	// final color
    vec3 col;
    col.r = texture( tex, vec2(uv.x+blur,uv.y) ).r;
    col.g = texture( tex, uv ).g;
    col.b = texture( tex, vec2(uv.x-blur,uv.y) ).b;
	
	// scanline
	float scanline = sin(uv.y*800.0)*0.04;
	col -= scanline;
	
	// vignette
	col *= 1.0 - d * 0.5;
	
    color = vec4(col,1.0);
}

