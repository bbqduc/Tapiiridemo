#version 150 core

uniform sampler2D texture;
uniform float time;
in vec2 texcoord;
out vec4 vFragColor;

void main(void)
{
	vFragColor=texture(texture, texcoord);
}
