//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#include <assert.h>

#include "ModelMan.h"

ModelMan::ModelMan()
{
	this->active = 0;
}

ModelMan * ModelMan::privGetInstance()
{
	// This is where its actually stored (BSS section)
	static ModelMan shaderMan;
	return &shaderMan;
}

void ModelMan::Create()
{
	ModelMan *pModelMan = ModelMan::privGetInstance();
	assert(pModelMan);
}

void ModelMan::Destroy()
{
	ModelMan *pModelMan = ModelMan::privGetInstance();
	assert(pModelMan);

	Model *pLink = pModelMan->active;

	while (pLink != nullptr)
	{
		Model *pTmp = pLink;
		pLink = (Model *)pLink->next;
		pModelMan->privRemove(pTmp, pModelMan->active);
		delete pTmp;
	}
}

void ModelMan::Add(Model *pModel)
{
	// Get the instance to the manager
	ModelMan *pModelMan = ModelMan::privGetInstance();
	assert(pModelMan);

	// Create a Model
	assert(pModel);

	// Now add it to the manager
	pModelMan->privAddToFront(pModel, pModelMan->active);
}

void ModelMan::privAddToFront(Model *node, Model *&head)
{
	assert(node != 0);

	if (head == 0)
	{
		head = node;
		node->next = 0;
		node->prev = 0;
	}
	else
	{
		node->next = head;
		head->prev = node;
		head = node;
	}
}

void ModelMan::privRemove(Model *pNode, Model *&poHead)
{
	assert(pNode);

	if (pNode->prev != nullptr)
	{	// middle or last node
		pNode->prev->next = pNode->next;
	}
	else
	{  // first
		poHead = (Model *)pNode->next;
	}

	if (pNode->next != nullptr)
	{	// middle node
		pNode->next->prev = pNode->prev;
	}

}

//--- End of File -------------------------------------------------------------
