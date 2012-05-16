#version 330 core

smooth in vec2 relativeToCenter;
in float z;
out vec4 vFragColor;

float gauss(float x, float a)
{
   return sqrt(a / 3.14159265) * exp(-a * x * x);
}

float blob(vec2 o, vec2 p)
{
   return gauss(distance(o, p), 50000.0);
}

uniform float time;
uniform vec4 basecolor;

void main(void)
{
	vec2 relativePosition = -1.0 + 2.0 * gl_FragCoord.xy / vec2(1024,768);
	float dist = relativeToCenter.x*relativeToCenter.x + relativeToCenter.y * relativeToCenter.y;
	dist *= 100;
	float mul = 1.0-dist;
	vFragColor = basecolor * vec4(dist+sin(time), abs(z)/100.0f+abs(relativePosition.x), abs(100-z)/100.0f+abs(relativePosition.y), 1.0-dist);

}
