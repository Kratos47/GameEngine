#ifndef FONT__H
#define FONT__H

#include "GameObject2DFont.h"

// Singleton

class FontLink
{
public:
	FontLink()
	{
		this->next = 0;
		this->prev = 0;
	}

	virtual ~FontLink()
	{

	}

	FontLink* next;
	FontLink* prev;
};



class Font : public FontLink
{
public:

	enum class Name
	{
		Text,
		Default,
		COUNTER,
		Uninitialized
	};


	Font(Name name, GameObject2DFont* font);

	void UpdateMessage(std::string str);

	// Big four
	Font() = delete;
	Font(const Font&) = delete;
	Font& operator=(Font&) = delete;
	virtual ~Font() override;

	// ----------------------------------------------------------------
	// Data 
	// ----------------------------------------------------------------
public:
	Name name;
	std::string str;
	GameObject2DFont* font;
};
#endif