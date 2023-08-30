#include "SSBO_MAN.h"


SSBOMan::SSBOMan()
{
	this->active = 0;
}

SSBOMan* SSBOMan::privGetInstance()
{
	// This is where its actually stored (BSS section)
	static SSBOMan shaderMan;
	return &shaderMan;
}



void SSBOMan::Create()
{
	SSBOMan* pShaderMan = SSBOMan::privGetInstance();
	assert(pShaderMan);
}

void SSBOMan::Destroy()
{
	SSBOMan* pShaderMan = SSBOMan::privGetInstance();
	assert(pShaderMan);

	SSBO* pLink = pShaderMan->active;

	while (pLink != nullptr)
	{
		SSBO* pTmp = pLink;
		pLink = (SSBO*)pLink->next;
		pShaderMan->privRemove(pTmp, pShaderMan->active);
		delete pTmp;
	}
}

SSBO* SSBOMan::Find(SSBO::Name reqName)
{
	SSBOMan* pShaderMan = SSBOMan::privGetInstance();
	assert(pShaderMan);

	SSBO* pLink = pShaderMan->active;

	while (pLink != nullptr)
	{
		if (pLink->ssboName == reqName)
			break;

		pLink = (SSBO*)pLink->next;
	}

	return pLink;
}


void SSBOMan::Add(SSBO* pSSBO)
{
	// Get the instance to the manager
	SSBOMan* pShaderMan = SSBOMan::privGetInstance();
	assert(pShaderMan);

	// Create a TextureNode
	assert(pSSBO);

	// Now add it to the manager
	pShaderMan->privAddToFront(pSSBO, pShaderMan->active);
}


void SSBOMan::privAddToFront(SSBO* node, SSBO*& head)
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

void SSBOMan::privRemove(SSBO* node, SSBO*& head)
{
	assert(node);

	if (node->prev != nullptr)
	{	// middle or last node
		node->prev->next = node->next;
	}
	else
	{  // first
		head = (SSBO*)node->next;
	}

	if (node->next != nullptr)
	{	// middle node
		node->next->prev = node->prev;
	}
}
