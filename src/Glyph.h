#ifndef GLYPH__H
#define GLYPH__H

#include "Rect.h"
#include "Texture.h"
// Singleton

class GlyphLink
{
public:
	GlyphLink()	
	{
		this->next = 0;
		this->prev = 0;
	}

	virtual ~GlyphLink() 
	{

	}

	GlyphLink* next;
	GlyphLink* prev;
};



class Glyph: public GlyphLink
{
public:

	enum class Name
	{
		Consolas36pt,
		NullObject,
		Uninitialized
	};

public:
	Glyph(Glyph::Name _name, const int _key, TextureNode* _texture, Rect rect);

	// Big four
	Glyph() = delete;
	Glyph(const Glyph&) = delete;
	Glyph& operator=(Glyph&) = delete;
	virtual ~Glyph() override;


	// ----------------------------------------------------------------
	// Data 
	// ----------------------------------------------------------------
public:
	Name name;
	int key;
	Rect pSubRect;
	TextureNode* pTexture;
};
#endif
