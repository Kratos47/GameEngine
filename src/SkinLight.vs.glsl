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
//uniform mat4 boneWorld[80];

layout (std430, binding = 5) buffer World
{
	mat4 boneWorld[];
};

uniform vec4 vLightPos;
uniform vec4 vColor;

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

	// Get vNorm
		mat4 mvMatrix = view_matrix * skin_matrix;
		mat4 mNormalMatrix;
		mNormalMatrix[0] = normalize(mvMatrix[0].xyzw);
		mNormalMatrix[1] = normalize(mvMatrix[1].xyzw);
		mNormalMatrix[2] = normalize(mvMatrix[2].xyzw);
		vec4 vNorm = normalize(mNormalMatrix * norms);

	// Get vLightDir
		vec4 ecPosition;
		vec4 ecPosition3;
		ecPosition = mvMatrix * vert;
		ecPosition3 = ecPosition.xyzw /ecPosition.w;
		vec4 vLightDir = normalize(vLightPos - ecPosition3);

		vec4 Lightpos = vec4(0,13,2,0);
	// Get vFragColor
		float fDot = max(0.0, dot(vNorm, vLightDir * Lightpos)); 
		vs_out.color.rgb = vColor.rgb * fDot;
		vs_out.color.a = vColor.a * fDot;


	// Remember: We use ROW major, but OpenGL GLSL multiplies in reverse order!
    gl_Position = proj_matrix * view_matrix * skin_matrix * vert;    
}

// --- End of File ----------------------------------------------------------- 