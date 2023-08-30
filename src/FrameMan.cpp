//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>


#include "FrameMan.h"
//
extern Frame_Bucket* pFrameHead;
//extern Frame_Bucket* pTmp;

FrameMan::FrameMan()
{
	this->active = 0;
}

FrameMan* FrameMan::privGetInstance()
{
	// This is where its actually stored (BSS section)
	static FrameMan AnimMan;
	return &AnimMan;
}


void FrameMan::Create()
{
	FrameMan* pAnimMan = FrameMan::privGetInstance();
	assert(pAnimMan);
}

void FrameMan::Destroy()
{
	FrameMan* pFrameMan = FrameMan::privGetInstance();
	assert(pFrameMan);

	Frame_Bucket* pLink = pFrameMan->active;

	while (pLink != nullptr)
	{
		Frame_Bucket* pTmp2 = pLink;
		pLink = (Frame_Bucket*)pLink->nextBucket;

		pFrameMan->privRemove(pTmp2, pFrameMan->active);

		delete pTmp2->pBone;
		delete pTmp2;
	}
}

void FrameMan::Add(Frame_Bucket* pAnim)
{
	// Get the instance to the manager
	FrameMan* pAnimMan = FrameMan::privGetInstance();
	assert(pAnimMan);

	// Create a TextureNode
	assert(pAnim);

	// Now add it to the manager
	pAnimMan->privAddToFront(pAnim, pAnimMan->active);
}

void FrameMan::privAddToFront(Frame_Bucket* node, Frame_Bucket*& head)
{
	assert(node != 0);

	if (head == 0)
	{
		head = node;
		node->nextBucket = 0;
		node->prevBucket = 0;
	}
	else
	{
		node->nextBucket = head;
		head->prevBucket = node;
		head = node;
	}
}

void FrameMan::privRemove(Frame_Bucket* pNode, Frame_Bucket*& poHead)
{
	assert(pNode);

	if (pNode->prevBucket != nullptr)
	{	// middle or last node
		pNode->prevBucket->nextBucket = pNode->nextBucket;
	}
	else
	{  // first
		poHead = (Frame_Bucket*)pNode->nextBucket;
	}

	if (pNode->nextBucket != nullptr)
	{	// middle node
		pNode->nextBucket->prevBucket = pNode->prevBucket;
	}

}

//--- End of File -------------------------------------------------------------
