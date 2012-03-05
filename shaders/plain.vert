#version 330 core

layout(location=0) in vec4 in_Position;

void main()
{
	gl_Position = vec4(in_Position.xyz, 1.0);
}
