//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

// TEXTURE RENDER - Fragment Shader

#version 400 core

// Not allowed in 400, only 420
// layout (binding = 0) uniform sampler2D tex_object;

// Uniforms
	uniform sampler2D tex_object;

// Varying
	in VS_OUT
	{
		vec2 textCoordinate;
	} fs_in;

	out vec4 color;

void main(void)
{
    color = texture(tex_object, fs_in.textCoordinate );
}

// --- End of File -----------------------------------------------------------
