#version 330 core

uniform float time;

out vec4 vFragColor;
void main(void)
{
	vec2 relativePosition = gl_FragCoord.xy / vec2(300, 300) + vec2(sin(time), cos(time));
	vFragColor = vec4(relativePosition.x, relativePosition.y, 1.0, 1.0);
}
