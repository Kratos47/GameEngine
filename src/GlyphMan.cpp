#include "GlyphMan.h"
#include "XmlLite.h"
#include "Shlwapi.h"
#include "Model.h"
#include "ShaderObject.h"
#include "Image.h"
#include "Texture.h"

GlyphMan::GlyphMan()
{
	this->active = 0;
}



void GlyphMan::Create()
{
	GlyphMan* pGlyphMan = GlyphMan::privGetInstance();
	assert(pGlyphMan);
}



void GlyphMan::Destroy()
{
	GlyphMan* pGlyphMan = GlyphMan::privGetInstance();
	assert(pGlyphMan);

	Glyph* pLink = pGlyphMan->active;

	while (pLink != nullptr)
	{
		Glyph* pTmp = pLink;
		pLink = (Glyph*)pLink->next;
		pGlyphMan->privRemove(pTmp, pGlyphMan->active);
		delete pTmp;
	}
}


void GlyphMan::privAddToFront(Glyph* node, Glyph*& head)
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

void GlyphMan::privRemove(Glyph* pNode, Glyph*& poHead)
{
	assert(pNode);

	if (pNode->prev != nullptr)
	{	// middle or last node
		pNode->prev->next = pNode->next;
	}
	else
	{  // first
		poHead = (Glyph*)pNode->next;
	}

	if (pNode->next != nullptr)
	{	// middle node
		pNode->next->prev = pNode->prev;
	}

}

Glyph* GlyphMan::Find(int _key)
{
	GlyphMan* pGlyphMan = GlyphMan::privGetInstance();
	assert(pGlyphMan);

	Glyph* pLink = pGlyphMan->active;

	while (pLink != nullptr)
	{
		Glyph* pTmp2 = pLink;
		pLink = (Glyph*)pLink->next;

		if (pTmp2->key == _key)
		{
			return pTmp2;
		}
	}

	return 0;
}

void GlyphMan::Add(Glyph::Name name, int key, Texture::Name textName, float x, float y, float width, float height)
{
	GlyphMan* pMan = GlyphMan::privGetInstance();

	TextureNode* tex = Texture::FindTex(textName);

	Glyph* pNode = new Glyph(name,key,tex,Rect(x,y,width,height));

	pMan->privAddToFront(pNode, pMan->active);
}


GlyphMan* GlyphMan::privGetInstance()
{
	// This is where its actually stored (BSS section)
	static GlyphMan pGlyphMan;
	return &pGlyphMan;
}



int ctoi(PCWSTR value, UINT valueSize)
{
	if (value == nullptr) 
		return 0;

	char* LN = new char[100];

	for (UINT i = 0; i <= valueSize; i++)
	{
		LN[i] = (char)value[i];

		if (i == valueSize)
		{
			LN[i] = (char)0;
		}
	}
	
	int ret = std::atoi((char*)LN);

	delete[] LN;

	return ret;
}

void GlyphMan::AddXml(Glyph::Name name, std::string assetName, Texture::Name textName)
{
	HRESULT hr;
	IXmlReader* reader = nullptr;
	IStream* readStream = nullptr;
	XmlNodeType nodeType;

	::CreateXmlReader(IID_PPV_ARGS(&reader), nullptr);
	SHCreateStreamOnFile((LPCSTR)assetName.c_str(), STGM_READ, &readStream);

	reader->SetProperty(XmlReaderProperty_DtdProcessing, DtdProcessing_Prohibit);
	reader->SetInput(readStream);


	//System.Xml.XmlTextReader reader = new XmlTextReader(assetName);

	char* LN = new char[100];

	PCWSTR prefix = nullptr;
	PCWSTR localName = nullptr;
	PCWSTR value = nullptr;

	UINT prefixSize = 0;
	UINT localNameSize = 0;
	UINT valueSize = 0;

	int key = -1;
	int x = -1;
	int y = -1;
	int width = -1;
	int height = -1;


	// I'm sure there is a better way to do this... but this works for now
	while (S_OK == (hr = reader->Read(&nodeType)))
	{
		switch (nodeType)
		{
			case XmlNodeType_Element: // The node is an element.
				reader->GetPrefix(&prefix, &prefixSize);
				reader->GetQualifiedName(&localName, &localNameSize);

				for (unsigned int i = 0; i <= localNameSize; i++)
				{
					LN[i] = (char)localName[i];

					if (i == localNameSize)
					{
						LN[i] = (char)0;
					}
				}

				if (strcmp(LN,"character") == 0)
				{
					reader->MoveToFirstAttribute();
					reader->GetValue(&value, &valueSize);
					key = ctoi(value, valueSize);
				}
				else if (strcmp(LN, "x") == 0)
				{
					while (S_OK == (hr = reader->Read(&nodeType)))
					{
						if (nodeType == XmlNodeType_Text)
						{
							reader->GetValue(&value, &valueSize);
							x = ctoi(value, valueSize);
							break;
						}
					}
				}
				else if (strcmp(LN, "y") == 0)
				{
					while (S_OK == (hr = reader->Read(&nodeType)))
					{
						if (nodeType == XmlNodeType_Text)
						{
							reader->GetValue(&value, &valueSize);
							y = ctoi(value, valueSize);
							break;
						}
					}
				}
				else if (strcmp(LN, "width") == 0)
				{
					while (S_OK == (hr = reader->Read(&nodeType)))
					{
						if (nodeType == XmlNodeType_Text)
						{
							reader->GetValue(&value, &valueSize);
							width = ctoi(value, valueSize);
							break;
						}
					}
				}
				else if (strcmp(LN, "height") == 0)
				{
					while (S_OK == (hr = reader->Read(&nodeType)))
					{
						if (nodeType == XmlNodeType_Text)
						{
							reader->GetValue(&value, &valueSize);
							height = ctoi(value, valueSize);
							break;
						}
					}
				}
				break;

			case XmlNodeType_EndElement:

				reader->GetPrefix(&prefix, &prefixSize);
				reader->GetQualifiedName(&localName, &localNameSize);
				for (unsigned int i = 0; i <= localNameSize; i++)
				{
					LN[i] = (char)localName[i];

					if (i == localNameSize)
					{
						LN[i] = (char)0;
					}
				}

			if (strcmp(LN, "character") == 0)
			{	
				//Swapped width and heigh around to make glyphs works maybe a hint for future bug
				GlyphMan::Add(name, key, textName, (float)x, (float)y, (float)width, (float)height);
			}
			break;
		}
	}
	delete[] LN;
}