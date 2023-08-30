//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

// COLOR RENDER - Vertex Shader

#version 430 core

// Attributes
layout (location = 0) in vec4 vert;// x y z (w = 1)
layout (location = 1) in vec4 uv;
layout (location = 2) in vec4 norms;

layout (location = 3) in vec4 boneWeights;
layout (location = 4) in uvec4 boneIndicies;

//Uniforms:
uniform mat4 proj_matrix;
uniform mat4 view_matrix;
uniform mat4 invM[80];
uniform mat4 boneWorld[80];

// Varying
	out VS_OUT
	{
		vec4 color;
	} vs_out;

void main(void)
{     
	mat4 skin_matrix = boneWorld[boneIndicies.x] * invM[boneIndicies.x]  * boneWeights.x
					  +boneWorld[boneIndicies.y] * invM[boneIndicies.y]  * boneWeights.y 
					  +boneWorld[boneIndicies.z] * invM[boneIndicies.z]  * boneWeights.z 
		               +boneWorld[boneIndicies.w] * invM[boneIndicies.w]  * boneWeights.w;
					 
	//mat4 skin_matrix = boneWorld[0] * invM[0] * boneWeights.x;

	// Remember: We use ROW major, but OpenGL GLSL multiplies in reverse order!
    gl_Position = proj_matrix * view_matrix * skin_matrix * vert;    
    //gl_Position = proj_matrix * view_matrix  * vert;    

	// Position is the color + Grey
    vs_out.color = vec4(0.0, 1.0, 0.0, 0.0);//vert * 2.0 + 
}

// --- End of File ----------------------------------------------------------- 