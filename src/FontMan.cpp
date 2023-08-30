#include "FontMan.h"

FontMan::FontMan()
{
	this->active = 0;
}

FontMan* FontMan::privGetInstance()
{
	// This is where its actually stored (BSS section)
	static FontMan shaderMan;
	return &shaderMan;
}

void FontMan::Create()
{
	FontMan* pFontMan = FontMan::privGetInstance();
	assert(pFontMan);
}

void FontMan::Destroy()
{
	FontMan* pFontMan = FontMan::privGetInstance();
	assert(pFontMan);

	Font* pLink = pFontMan->active;

	while (pLink != nullptr)
	{
		Font* pTmp = pLink;
		pLink = (Font*)pLink->next;
		pFontMan->privRemove(pTmp, pFontMan->active);
		delete pTmp;
	}
}

void FontMan::Add(Font::Name name, GameObject2DFont* pGameObject2DFont)
{
	FontMan* pMan = FontMan::privGetInstance();

	Font* pNode = new Font(name, pGameObject2DFont);

	pMan->privAddToFront(pNode, pMan->active);
}

void FontMan::Remove(Font* pNode)
{
	assert(pNode != nullptr);
	FontMan* pMan = FontMan::privGetInstance();

	pMan->privRemove(pNode, pMan->active);
}


Font* FontMan::Find(Font::Name _name)
{
	FontMan* pFontMan = FontMan::privGetInstance();
	assert(pFontMan);

	Font* pLink = pFontMan->active;

	while (pLink != nullptr)
	{
		Font* pTmp2 = pLink;
		pLink = (Font*)pLink->next;

		if (pTmp2->name == _name)
		{
			return pTmp2;
		}
	}

	return 0;
}

void FontMan::Update(Font::Name name, std::string str)
{
	FontMan* pMan = FontMan::privGetInstance();

	Font* font = pMan->Find(name);

	assert(font);

	font->UpdateMessage(str);
}


void FontMan::privAddToFront(Font* node, Font*& head)
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

void FontMan::privRemove(Font* pNode, Font*& poHead)
{
	assert(pNode);

	if (pNode->prev != nullptr)
	{	// middle or last node
		pNode->prev->next = pNode->next;
	}
	else
	{  // first
		poHead = (Font*)pNode->next;
	}

	if (pNode->next != nullptr)
	{	// middle node
		pNode->next->prev = pNode->prev;
	}

}
