#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices=4) out;
smooth out vec2 relativeToCenter;

void main()
{
	vec4 point = gl_in[0].gl_Position;
	relativeToCenter = vec2(-0.1f, -0.1f);
	
	gl_Position = point + vec4(-0.1f,-0.1f,0,0);
	EmitVertex();
	
	relativeToCenter = vec2(0.1f, -0.1f);
	gl_Position = point + vec4(0.1f,-0.1f,0,0);
	EmitVertex();
	
	relativeToCenter = vec2(-0.1f, 0.1f);
	gl_Position = point + vec4(-0.1f,0.1f,0,0);
	EmitVertex();
	
	relativeToCenter = vec2(0.1f, 0.1f);
	gl_Position = point + vec4(0.1f,0.1f,0,0);
	EmitVertex();

	EndPrimitive();
}
