#version 330 core

uniform sampler2D texture;
uniform float time;
in vec2 texcoord;
out vec4 vFragColor;

void main(void)
{
	vFragColor=texture2D(texture, texcoord);
}
