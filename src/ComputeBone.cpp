#include "ComputeBone.h"


ComputeBone::ComputeBone(int numBones)
{
	this->pBoneData = nullptr;
	this->boneCount = numBones;
	this->numFrames = 0;
}

ComputeBone::~ComputeBone()
{
	delete[] this->pBoneData;
}

ComputeBone::ComputeBone(Frame_Bucket* pFrameBucket, int numBones)
{
	assert(pFrameBucket);

	// How many bones?
	this->boneCount = numBones;

	// How many frames?
	unsigned int frameCount = 0;
	Frame_Bucket* pTmp = pFrameBucket;

	while (pTmp != nullptr)
	{
		frameCount++;
		pTmp = pTmp->nextBucket;
	}

	// Remember the 1st frame is the result... so skip that
	this->numFrames = frameCount - 1;

	// Allocate the number of bones * number of frames

	this->pBoneData = new Bone[this->numFrames * this->boneCount]();

	Frame_Bucket*pFrame = pFrameBucket;
	assert(pFrame);

	// skip first frame its the result not data
	pFrame = pFrame->nextBucket;
	assert(pFrame);

	// Big HACK
	// Since I'm skipping the first result frame...  protect
	Bone* pDestBone = this->pBoneData;
	Bone* pSrcBone = nullptr;

	while (pFrame != nullptr)
	{
		pSrcBone = pFrame->pBone;

		// copy all the bones
		for (unsigned int i = 0; i < this->boneCount; i++)
		{
			pDestBone[i] = pSrcBone[i];
		}

		pFrame = pFrame->nextBucket;
		pDestBone += this->boneCount;
	}

}