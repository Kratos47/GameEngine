#ifndef ANIM_STL_H
#define ANIM_STL_H

// Keenan(3.2.4) - STL

#include <assert.h>
#include <vector>
#include <algorithm>


struct SKLTN
{
	char nodeName[64];
	unsigned int currNodeIndex = 0;
	unsigned int parentNodeIndex = 0;
	unsigned int NumBones = 0;
};

struct ClipHdr
{
	char clipName[64];
	unsigned int frameCount;
	unsigned int clipStartIndex;
	unsigned int clipEndIndex;
	unsigned int totalTime;
	unsigned int numclips;
	unsigned int currentClipIndex;
};

struct ClipData
{
	Vect trans;
	Quat rot;
	Vect scale;
	unsigned int keyTime;
	unsigned int msTime;
};

#endif