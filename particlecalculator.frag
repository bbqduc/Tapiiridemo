#version 330 core

uniform float dt;
uniform sampler1D velocityTexture;
uniform sampler1D positionTexture;

in float texcoord;



void main()
{
	gl_FragColor = texture1D(positionTexture, texcoord) + dt * texture1D(velocityTexture, texcoord);
}