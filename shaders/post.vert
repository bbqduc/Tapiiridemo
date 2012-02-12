#version 330 core

layout(location=0) in vec2 v_coord;
uniform sampler2D texture;
out vec2 texcoord;
 
void main(void) 
{
	gl_Position = vec4(v_coord, 0.0, 1.0);
	texcoord = (v_coord + 1.0) / 2.0;
}
