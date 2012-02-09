#version 330 core

in vec2 centerPosition;
out vec4 vFragColor;

float gauss(float x, float a)
{
   return sqrt(a / 3.14159265) * exp(-a * x * x);
}

float blob(vec2 o, vec2 p)
{
   return gauss(distance(o, p), 50000.0);
}

void main(void)
{
	vec2 relativePosition = -1.0 + 2.0 * gl_FragCoord.xy / vec2(1024,768);
	vec2 relativeCenterPosition = -1.0 + 2.0 * centerPosition / vec2(1024,768);
	float dist = distance(centerPosition, relativePosition);
	vFragColor = vec4(centerPosition.x, centerPosition.y, dist, 0.2); //vec4(vec3(dist/2), 1.0f);//vec4(sin(dist), cos(dist), 0.0, gauss(dist, 2.0)); //vec4(relativePosition.x, relativePosition.y, 1.0, 1.0);
}
