#version 330 core

uniform float time;

layout(location=0) in vec3 in_Position;

smooth out float distance;
void main()
{
	distance = abs(in_Position.x);//abs(sin(time));
	gl_Position = vec4(in_Position.x, in_Position.y, in_Position.z - distance, 1.0);
}
