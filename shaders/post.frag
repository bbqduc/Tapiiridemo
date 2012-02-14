#version 330 core

uniform sampler2D textureID;
uniform float time;
in vec2 texcoord;
out vec4 vFragColor;

void main(void)
{
	vFragColor=0.5-texture(textureID, texcoord);
}
