#version 330 core

out vec4 vFragColor;
void main(void)
{
	vec2 relativePosition = gl_FragCoord.xy / vec2(1024, 768);
	vFragColor = vec4(relativePosition.x, relativePosition.y, 1.0, 1.0);
}
