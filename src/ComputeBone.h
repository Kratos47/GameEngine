#ifndef COMPUTE_BONE
#define COMPUTE_BONE


#include "FrameBucket.h"

struct ComputeBone : public Align16
{
public:
	ComputeBone(int numBones);
	ComputeBone(const ComputeBone&) = delete;
	ComputeBone& operator = (const ComputeBone&) = delete;
	~ComputeBone();

	ComputeBone(Frame_Bucket* pFrameBucket, int numBones);

// ---------------------------------------
// Data
// ---------------------------------------
public:
	Bone *pBoneData;
	unsigned int boneCount;
	unsigned int numFrames;
};




#endif