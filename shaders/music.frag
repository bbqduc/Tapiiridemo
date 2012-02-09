#version 330 core

uniform float time;

out vec4 vFragColor;
smooth in float distance;

void main(void)
{
	vec2 relativePosition = gl_FragCoord.xy / vec2(300, 300) + vec2(sin(time), cos(time));
	vFragColor = vec4(relativePosition.x*time*0.5, relativePosition.y*time*0.2, time*0.8, 1.0);
}
