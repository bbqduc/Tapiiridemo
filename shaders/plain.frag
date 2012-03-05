#version 330 core

smooth in vec2 relativeToCenter;
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
uniform int pos;

void main(void)
{
	vec2 relativePosition = -1.0 + 2.0 * gl_FragCoord.xy / vec2(1024,768);
	float dist = relativeToCenter.x*relativeToCenter.x + relativeToCenter.y * relativeToCenter.y;
	dist *= 100;
	float mul = 1.0-dist;

	vFragColor = vec4(mul*(2.5-pos)*abs(relativePosition.x)*abs(sin(time)), mul*(3.5-pos)*abs(relativePosition.y), mul*(pos+0.5)*abs(relativePosition.x+relativePosition.y), 1.0+dist);

}
