//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

// COLOR RENDER - Fragment Shader

#version 400 core

// Varying
	out vec4 color;

	in VS_OUT
	{
		vec4 color;
	} fs_in;


void main(void)
{
    color = fs_in.color;
}

// --- End of File -----------------------------------------------------------
