#version 330 core

uniform mat4 MVP;

layout(location=0) in vec3 in_Position;

void main()
{
	gl_Position = MVP * vec4(in_Position, 1.0);
}
