#version 330 core

uniform mat4 MVP;

layout(location=0) in vec3 in_Position;
out vec3 position;

void main()
{
	position = normalize(vec3(MVP[3]));
	gl_Position = MVP * vec4(in_Position, 1.0);
}
