#version 330 core

uniform float time;

out vec4 vFragColor;
smooth in float distance;

float gauss(float x, float a)
{
   return sqrt(a / 3.14159265) * exp(-a * x * x);
}

void main(void)
{
	vec2 relativePosition = gl_FragCoord.xy / vec2(300, 300) + vec2(sin(time), cos(time));
	float blur = gauss(2, distance);
	vFragColor = vec4(relativePosition.x, relativePosition.y, 1.0, 1.0)*blur;
}
