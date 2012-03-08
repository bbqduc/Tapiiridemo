#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices=4) out;
smooth out vec2 relativeToCenter;
out float z;

void main()
{
	vec4 point = gl_in[0].gl_Position;
	z = point.z;
	relativeToCenter = vec2(-0.5f, -0.5f);
	
	gl_Position = point + vec4(-0.5f,-0.5f,0,0);
	EmitVertex();
	
	relativeToCenter = vec2(0.5f, -0.5f);
	gl_Position = point + vec4(0.5f,-0.5f,0,0);
	EmitVertex();
	
	relativeToCenter = vec2(-0.5f, 0.5f);
	gl_Position = point + vec4(-0.5f,0.5f,0,0);
	EmitVertex();
	
	relativeToCenter = vec2(0.5f, 0.5f);
	gl_Position = point + vec4(0.5f,0.5f,0,0);
	EmitVertex();

	EndPrimitive();
}
