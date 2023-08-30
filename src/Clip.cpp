#include "Clip.h"
#include "FrameBucket.h"
#include "FrameMan.h"
#include "GameTime.h"
Clip::Clip()
{
	this->pNext = 0;
	this->pPrev = 0;
	this->pFrameHead = 0;
	this->pClipFrames = 0;
	this->maxTime = Time(Time::ZERO);
	this->clipName = Clip::ClipName::DEFAULT;
}
Clip::~Clip()
{
	FrameMan* pFrameMan = FrameMan::privGetInstance();
	assert(pFrameMan);

	Frame_Bucket* pLink = this->pFrameHead;
	Frame_Bucket* pTmp2 = 0;

	while (pLink != nullptr)
	{
		pTmp2 = pLink;
		pLink = (Frame_Bucket*)pLink->nextBucket;
		delete[] pTmp2->pBone;
		pFrameMan->privRemove(pTmp2, pFrameMan->active);
	}
	delete this->pFrameHead;
	delete[] this->pClipFrames;
}

Clip::Clip(Frame_Bucket* FrameHead, Frame_Bucket* ClipFrames, const char* clipName, Time MaxTime)
{
	this->pNext = 0;
	this->pPrev = 0;
	this->pFrameHead = FrameHead;
	this->pClipFrames = ClipFrames;
	this->clipName = charName2Enum(clipName);
	this->maxTime = MaxTime;
}

Clip::ClipName Clip::charName2Enum(const char* pclipName)
{
	if (strcmp(pclipName, "run") == 0) 
	{
		return Clip::ClipName::HUMAN_RUN;
	}
	else if (strcmp(pclipName, "punch") == 0)
	{
		return Clip::ClipName::HUMAN_PUNCH;
	}
	else if (strcmp(pclipName, "idle") == 0)
	{
		return Clip::ClipName::TEDDY_IDLE;
	}
	else if (strcmp(pclipName, "walk") == 0)
	{
		return Clip::ClipName::TEDDY_WALK;
	}
	else if (strcmp(pclipName, "shot") == 0)
	{
		return Clip::ClipName::HUMAN_SHOT;
	}
	else if (strcmp(pclipName, "Generic_Run") == 0)
	{
		return Clip::ClipName::GENERIC_RUN;
	}
	else if (strcmp(pclipName, "Hero_Walk") == 0)
	{
		return Clip::ClipName::HERO_WALK;
	}
	else if (strcmp(pclipName, "Hero_Run_fast") == 0)
	{
		return Clip::ClipName::HERO_RUN_FAST;
	}
	return Clip::ClipName::DEFAULT;
}
