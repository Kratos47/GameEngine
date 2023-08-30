#include "Font.h"

Font::Font(Name name, GameObject2DFont* font)
{
	this->name = name;
	this->str = font->pMessege;
	this->font = font;
}

void Font::UpdateMessage(std::string str)
{
	this->font->UpdateMessege(str);
}

Font::~Font()
{
}
