#version 400 core

// Varying
	in VS_OUT
	{
		vec4 vFragColor;
	} fs_in;

	out vec4 color;

void main(void) 
{ 
	color = fs_in.vFragColor;
}
