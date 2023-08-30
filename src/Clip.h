#ifndef CLIP_H
#define CLIP_H

struct Frame_Bucket;
#include "GameTime.h"
class Clip 
{
public:

	enum ClipName
	{
		HUMAN_RUN,
		HUMAN_PUNCH,
		HUMAN_SHOT,
		TEDDY_WALK,
		TEDDY_IDLE,
		GENERIC_RUN,
		HERO_WALK,
		HERO_RUN_FAST,
		DEFAULT
	};

	// Big four    // Keenan(20)
	Clip();
	Clip(const Clip&) = delete;
	Clip& operator=(Clip&) = delete;
	virtual ~Clip();

	Clip(Frame_Bucket* pFrameHead, Frame_Bucket* pClipFrames, const char* clipName,Time maxTime);

	Clip::ClipName charName2Enum(const char* clipName);

	//Data
	Frame_Bucket* pFrameHead;
	Frame_Bucket* pClipFrames;
	Clip::ClipName clipName;
	Time maxTime;

	Clip* pNext;
	Clip* pPrev;
};
#endif