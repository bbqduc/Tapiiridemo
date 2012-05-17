#version 330 core

uniform float time;
uniform float tunnelDistance;

out vec4 fragColor;

void main()
{
	const vec2 resolution = vec2(1024, 768);

	vec2 pos = gl_FragCoord.xy / resolution - 0.5;

	pos.x *= (resolution.x / resolution.y);

	float v = atan(pos.y+(sin(time/20)), pos.x);
	float u = length(pos) - tunnelDistance;
	u = max(u,0);

	float t = time / 0.5 + 1.0 / u;

	float intensity = abs(sin(t*10.0+v)+sin(v*8.0));

	vec3 col = vec3(1.0, 0.5, 0.0);

	fragColor = vec4(u * col * intensity * (u*3.0), 1.0);
}
