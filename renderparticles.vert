#version 330 core

uniform sampler1D texture;
uniform mat4 MVP;

in float texcoord;

out vec4 particlePos;

void main()
{
	particlePos = MVP * texture1D(texture, texcoord);
}