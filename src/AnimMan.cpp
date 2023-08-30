#include "AnimMan.h"

AnimMan::AnimMan()
{
	this->active = 0;
}

AnimMan* AnimMan::privGetInstance()
{
	// This is where its actually stored (BSS section)
	static AnimMan ClipMan;
	return &ClipMan;
}

AnimController* AnimMan::Find(AnimController::InstancenName name)
{
	AnimMan* pAnimMan = AnimMan::privGetInstance();
	assert(pAnimMan);

	AnimController* pLink = pAnimMan->active;

	while (pLink != nullptr)
	{
		AnimController* pTmp2 = pLink;
		pLink = (AnimController*)pLink->pNext;

		if (pTmp2->pName == name)
		{
			return pTmp2;
		}
	}

	return 0;
}


void AnimMan::Create()
{
	AnimMan* pAnimMan = AnimMan::privGetInstance();
	assert(pAnimMan);
}

void AnimMan::Destroy()
{
	AnimMan* pAnimMan = AnimMan::privGetInstance();
	assert(pAnimMan);

	AnimController* pLink = pAnimMan->active;

	while (pLink != nullptr)
	{
		AnimController* pTmp2 = pLink;

		pLink = (AnimController*)pLink->pNext;

		delete pTmp2->pSkeleton;
		if(pTmp2->bonePoseInv != (Matrix*)0xcdcdcdcd)
		delete[] pTmp2->bonePoseInv;
		if (pTmp2->currPose != (Matrix*)0xcdcdcdcd)
		delete[] pTmp2->currPose;
		delete pTmp2;
	}
}

void AnimMan::Add(AnimController* pAnimController)
{
	// Get the instance to the manager
	AnimMan* pAnimMan = AnimMan::privGetInstance();
	assert(pAnimMan);

	// Create a TextureNode
	assert(pAnimController);

	// Now add it to the manager
	pAnimMan->privAddToFront(pAnimController, pAnimMan->active);
}

void AnimMan::privAddToFront(AnimController* node, AnimController*& head)
{
	assert(node != 0);

	if (head == 0)
	{
		head = node;
		node->pNext = 0;
		node->pPrev = 0;
	}
	else
	{
		node->pNext = head;
		head->pPrev = node;
		head = node;
	}
}