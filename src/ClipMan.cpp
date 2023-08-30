#include "ClipMan.h"

ClipMan::ClipMan()
{
	this->active = 0;
}

ClipMan* ClipMan::privGetInstance()
{
	// This is where its actually stored (BSS section)
	static ClipMan ClipMan;
	return &ClipMan;
}

Clip* ClipMan::Find(Clip::ClipName name)
{
	ClipMan* pClipMan = ClipMan::privGetInstance();
	assert(pClipMan);

	Clip* pLink = pClipMan->active;

	while (pLink != nullptr)
	{
		Clip* pTmp2 = pLink;
		pLink = (Clip*)pLink->pNext;

		if (pTmp2->clipName == name) 
		{
			return pTmp2;
		}
	}

	return 0;
}


void ClipMan::Create()
{
	ClipMan* pClipMan = ClipMan::privGetInstance();
	assert(pClipMan);
}

void ClipMan::Destroy()
{
	ClipMan* pClipMan = ClipMan::privGetInstance();
	assert(pClipMan);

	Clip* pLink = pClipMan->active;

		while (pLink != nullptr)
		{
			Clip* pTmp2 = pLink;

			//don't quite understand how i fixed this:: i call this intuition coding
			if ((Clip*)pLink->pNext != nullptr) 
			{
				pLink = (Clip*)pLink->pNext;
			}
			else 
			{
				pLink = (Clip*)pLink->pPrev;
			}
			pClipMan->privRemove(pTmp2, pClipMan->active);

			delete pTmp2;
		}
}

void ClipMan::Add(Clip* pClip)
{
	// Get the instance to the manager
	ClipMan* pClipMan = ClipMan::privGetInstance();
	assert(pClipMan);

	// Create a TextureNode
	assert(pClip);

	// Now add it to the manager
	pClipMan->privAddToFront(pClip, pClipMan->active);
}

void ClipMan::privAddToFront(Clip* node, Clip*& head)
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

void ClipMan::privRemove(Clip* pNode, Clip*& poHead)
{
	assert(pNode);

	if (pNode->pPrev != nullptr)
	{	// middle or last node
		pNode->pPrev->pNext = pNode->pNext;
	}
	else
	{  // first
		poHead = (Clip*)pNode->pNext;
	}

	if (pNode->pNext != nullptr)
	{	// middle node
		pNode->pNext->pPrev = pNode->pPrev;
	}

}