#ifndef FRAME_BUCKET
#define FRAME_BUCKET

#include "Timer.h"
#include "Vect.h"
#include "Quat.h"
#include "FrameLink.h"
struct Bone : public Align16
{
	Vect  T;
	Quat  Q;
	Vect  S;
};

struct Frame_Bucket: FrameLink
{
	Frame_Bucket *nextBucket;
	Frame_Bucket *prevBucket;
	Time		  KeyTime;
	Bone		  *pBone;
	char		  pad[4];
};


#endif