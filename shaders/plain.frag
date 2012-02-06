#version 330 core

out vec4 vFragColor;
void main(void)
{
	vec2 relativePosition = gl_FragCoord.xy / vec2(300, 300);
	vFragColor = vec4(asd.x, asd.y, 1.0, 1.0);
}
