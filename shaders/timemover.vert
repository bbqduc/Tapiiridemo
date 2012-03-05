#version 330 core

uniform float time;

layout(location=0) in vec4 in_Position;

smooth out float dist;
void main()
{
	dist = in_Position.x*in_Position.x + in_Position.y*in_Position.y;
	gl_Position = vec4(in_Position.x, in_Position.y, in_Position.z, 1.0);
}
