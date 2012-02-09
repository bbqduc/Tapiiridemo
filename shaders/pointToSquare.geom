#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices=4) out;
out vec3 centerPosition;

void main()
{
	vec4 point = gl_in[0].gl_Position;
	centerPosition = vec3(point);
	
	gl_Position = point + vec4(-0.1f,-0.1f,0,0);
	EmitVertex();

	gl_Position = point + vec4(0.1f,-0.1f,0,0);
	EmitVertex();

	gl_Position = point + vec4(-0.1f,0.1f,0,0);
	EmitVertex();

	gl_Position = point + vec4(0.1f,0.1f,0,0);
	EmitVertex();

	EndPrimitive();
}
