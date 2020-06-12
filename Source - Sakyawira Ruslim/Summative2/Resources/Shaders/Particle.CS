#version 450 core
#define WORK_GROUP_SIZE 128

layout(local_size_x = WORK_GROUP_SIZE, local_size_y = 1, local_size_z = 1) in;

layout(std430, binding = 0) buffer posBuffer { vec4 pos []; };
layout(std430, binding = 1) buffer initialPosBuffer { vec4 initialPos []; };
layout(std430, binding = 2) buffer velBuffer { vec4 vel []; };
layout(std430, binding = 3) buffer initialVelBuffer { vec4 initialVel []; };

uniform float currentTime;

void main()
{
	uint i = gl_GlobalInvocationID.x;

	vec3 gravity = vec3(sin(currentTime * i) / 100.0f, -0.13f * .0167f, cos(currentTime * i) / 100.0f);
	vel[i].xyz += gravity;

	pos[i].xyz += vel[i].xyz;
	pos[i].w -= 2.6f * .167f;

	if (pos[i].w <= 0.0f || pos[i].y < -90.0f)
	{
		pos[i] = initialPos[i];
		pos[i].w = initialVel[i].w;

		vel[i] = initialVel[i];
	}
}