#ifndef GLYPH_MANAGER_H
#define GLYPH_MANAGER_H

#include "Texture.h"
#include "Glyph.h"
class Model;
class ShaderObject;
// Singleton
class GlyphMan
{
public:
	GlyphMan();
	static void Create();
	static void Destroy();

	void privAddToFront(Glyph* node, Glyph*& head);

	void privRemove(Glyph* pNode, Glyph*& poHead);

	static Glyph* Find(int _key);

	static void Add(Glyph::Name name, int key, Texture::Name textName, float x, float y, float width, float height);

	static GlyphMan* privGetInstance();
	
	static void GlyphMan::AddXml(Glyph::Name name, std::string assetName, const Texture::Name _texName);
public:  // add

	Glyph* active;
};
#endif
