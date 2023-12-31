//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "sb7.h"

#include "TextureNodeLink.h"

class TextureNode;

class Texture
{
public:
	static const unsigned int ASSET_NAME_SIZE = 64;

	enum class Name
	{
		STONES,
		RED_BRICK,
		DUCKWEED,
		ROCKS,
		FOOTBALL,
		SPACE_FRIGATE,
		DEFAULT,
		TEDDY,
		ASTROBOY,
		BUGGY,
		WARBEAR,
		POLAR_BEAR,
		INVADERS,
		Consolas36pt,
		NOT_INITIALIZED
	};

public:	
	Texture(const Texture &) = delete;
	Texture & operator = (const Texture &) = delete;
	~Texture() = default;

	static void Destroy();
	static void Add( const char * const _assetName, const Texture::Name _name,
		GLenum minFilter = GL_LINEAR,
		GLenum magFilter = GL_LINEAR,
		GLenum horizWrapMode = GL_CLAMP_TO_EDGE,
		GLenum vertWrapMode = GL_CLAMP_TO_EDGE);

	static void Add(unsigned char* TexBuffer, const char* _assetName, const Texture::Name _name,
		GLenum minFilter = GL_LINEAR, 
		GLenum magFilter = GL_LINEAR,
		GLenum horizWrapMode = GL_CLAMP_TO_EDGE, 
		GLenum vertWrapMode = GL_CLAMP_TO_EDGE);

	static void DeleteAllTextures();
	static GLuint Find( const Texture::Name _name);

	static TextureNode* FindTex(const Texture::Name _name);

private:  
	// methods
	Texture();
	static Texture *privGetInstance();

	// helper private methods
	void privLoadTexture( const char * const _assetName, GLuint *&textureID );
	void privLoadTexture(const char* const _assetName, GLuint*& textureID, int& width, int& height);
	void privLoadTexture(unsigned char* const TexData, GLuint*& textureID);
	bool privLoadTGATexture(const char* szFileName, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode, int& nWidth, int& nHeight);
	bool privLoadTGATexture(const char *szFileName, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode);
	bool privLoadTGATexture(unsigned char* TexData, GLint minFilter, GLint magFilter, GLint horizWrapMode, GLint vertWrapMode);
	void privAddToFront(TextureNodeLink *node, TextureNodeLink *&head);
	GLbyte *gltReadTGABits(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat);

	GLbyte* gltReadTGABits(unsigned char* TexData, GLint* iWidth, GLint* iHeight, GLint* iComponents, GLenum* eFormat);

private: 

	TextureNodeLink *active;
};

#endif

// --- End of File --------------------------------------------------
