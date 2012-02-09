#version 330 core

uniform float time;

out vec4 vFragColor;
smooth in float distance;
in float t;

float gauss(float x, float a)
{
   return sqrt(a / 3.14159265) * exp(-a * x * x);
}

void main(void)
{
	vec2 relativePosition = gl_FragCoord.xy / vec2(300, 300) + vec2(sin(time), cos(time));
	float blur = gauss(2, distance);
	vFragColor = vec4(relativePosition.x*time*0.5,time*0.2,relativePosition.y*time*0.8,1.0);
}
