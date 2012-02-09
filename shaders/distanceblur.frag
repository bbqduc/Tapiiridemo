#version 330 core

uniform float time;

out vec4 vFragColor;
smooth in float dist;

float gauss(float x, float a)
{
   return sqrt(a / 3.14159265) * exp(-a * x * x);
}

void main(void)
{
	vFragColor = vec4(1.0);//sin(time*0.1f), cos(time*0.1f), 0, 1);
	vec2 relativePosition = gl_FragCoord.xy / vec2(1024, 768) - vec2(0.5);
	float dist2 = relativePosition.x*relativePosition.x + relativePosition.y*relativePosition.y;
	float blur = gauss(sqrt(dist2), 1);
	vFragColor *= blur;
}
