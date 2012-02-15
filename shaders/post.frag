#version 330 core

uniform sampler2D textureID;
uniform float time;
in vec2 texcoord;
out vec4 vFragColor;

const int MAXI=100;
float mandelbrot(vec2 p)
{
	float x=0; float y=0;
	p.y-=0.2;

	int i=0;
	while(x*x+y*y<4 && i<MAXI)
	{
		float xtemp=x*x-y*y-p.x+1.0;
		y=2*x*y+p.y-1.2;
		x=xtemp;
		i+=1;
	}
	if(i==MAXI) return 0.0;
	return float(i)/float(MAXI);
}

void main(void)
{
	vec2 point=vec2(gl_FragCoord.x/1024, gl_FragCoord.y/768);
	float c = mandelbrot(point*vec2(3.0));
	//vFragColor=mix(texture(textureID, texcoord), vec4(c, 0.0, 0.0, 1.0), 0.5);
	vFragColor=vec4(c, 0.0, 0.0, 1.0);
}
