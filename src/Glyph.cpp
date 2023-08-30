#include "Glyph.h"
#include "Rect.h"
#include"Texture.h"

Glyph::Glyph(Glyph::Name _name, const int _key, TextureNode* _texture, Rect rect)
{
	name = _name;
	key = _key;
	pSubRect = rect;
	pTexture = _texture;
}

Glyph::~Glyph()
{

}
