#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "Font.h"
#include "Texture.h"
#include "GameObject2DFont.h"
// Singleton
class FontMan
{
public:
	
	// Big four
	FontMan();
	FontMan(const FontMan&) = delete;
	FontMan& operator=(FontMan&) = delete;
	~FontMan() = default;
	
	static void Add(Font::Name name, GameObject2DFont *pGameObject2DFont);
	
	static Font* Find(Font::Name name);
	
	static void Update(Font::Name name, std::string str);

	static void Create();
	static void Destroy();

	// methods

	static FontMan* privGetInstance();
	static void Remove(Font* pNode);

	void privAddToFront(Font* node, Font*& head);
	void privRemove(Font* node, Font*& head);

private:  // add


	//----------------------------------------------------------------------
	// Data
	//----------------------------------------------------------------------
	Font* active;

};
#endif


