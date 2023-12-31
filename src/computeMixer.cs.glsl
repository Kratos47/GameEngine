#version 430 core

// ------------------------------------------
//    Compute Mixer
// ------------------------------------------

// localize breakdown
layout(local_size_x = 20) in;


struct Bone
{
	vec4 t;
	vec4 q;
	vec4 s;
};


layout (std430, binding = 0) buffer KeyFrameA
{
	Bone KeyA_array[];
};

layout (std430, binding = 1) buffer KeyFrameB
{
	Bone KeyB_array[];
};

layout (std430, binding = 2) buffer Result
{
	Bone Result_array[];
};


// Uniforms
// ...

uniform float ts;
uniform int boneCount;

// Function
vec4 slerp( vec4 src, vec4 tar, float t);

void main()
{
	uint boneIndex = gl_GlobalInvocationID.x;

	if( gl_GlobalInvocationID.x < boneCount )
	{

		vec4 trans = mix( KeyA_array[boneIndex].t, KeyB_array[boneIndex].t, ts );
		vec4 quat = slerp( KeyA_array[boneIndex].q, KeyB_array[boneIndex].q, ts );
		vec4 scale = mix( KeyA_array[boneIndex].s, KeyB_array[boneIndex].s, ts );

		Result_array[boneIndex].t = trans;
		Result_array[boneIndex].q = quat;
		Result_array[boneIndex].s = scale;
	}
};


vec4 slerp( vec4 qsrc, vec4 qtar, float t)
{
	vec4 qresult;

	float tmpcos = dot(qsrc, qtar);
	
	//test VAl
	//float epsilon = 1222.5f;
	
	//Current VAl
	float epsilon = 0.00001f;

	//Orignal Value
	//float epsilon = 0.001f;

	if( (epsilon + 1.0f >= tmpcos) && (1.0f - epsilon <= tmpcos) )
	{
		return qsrc;
	}

	if(tmpcos < 0.0f )
	{
		tmpcos = -tmpcos;
		qtar = -qtar;
	}

	float angle = acos(tmpcos);
	
	float sintO = sin( t * angle );
	float sin1minustO = sin( (1.0 - t)*angle );
	float sinO = sin( angle );

	if ( abs(angle) < 0.001f )
	{
		// snap to src
		qresult = qsrc;
	}
	else
	{
		qresult = (sin1minustO/sinO)*qsrc + (sintO/sinO)*qtar;
	}

	return qresult;

}
