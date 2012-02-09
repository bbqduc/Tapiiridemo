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

	if(pos==0)
		vFragColor = vec4(mul * relativePosition.x * time, mul * relativePosition.y, mul * abs(relativePosition.x+relativePosition.y), 1.0-dist);
	else if(pos==1)
		vFragColor = vec4(mul * relativePosition.x, mul * relativePosition.y * time, mul * abs(relativePosition.x+relativePosition.y), 1.0-dist);
	else if(pos==2)
		vFragColor = vec4(mul * relativePosition.x, 1.0 * -relativePosition.x, mul * abs(relativePosition.x+relativePosition.y), 1.0-dist);
}
