#version 330 core

in vec3 centerPosition;
out vec4 vFragColor;

float gauss(float x, float a)
{
   return sqrt(a / 3.14159265) * exp(-a * x * x);
}

void main(void)
{
	vec2 relativePosition = gl_FragCoord.xy / vec2(1024, 768);
	float dist = distance(centerPosition.xy, relativePosition.xy);
	vFragColor = vec4(vec3(gauss(dist, 2.0)), 1.0);//vec4(1.0f, 1.0f, 1.0f, gauss(dist, 2.0)); //vec4(relativePosition.x, relativePosition.y, 1.0, 1.0);
}
