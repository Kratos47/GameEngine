//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GraphicsObjectFont_H
#define GraphicsObjectFont_H

#include "Texture.h"
#include "Image.h"
#include "ShaderObject.h"
#include "GraphicsObject.h"
#include "Glyph.h"

class GraphicsObjectFont : public GraphicsObject
{
public:
	GraphicsObjectFont(Model* pModel, ShaderObject* pShaderObj);
	virtual ~GraphicsObjectFont()override;

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	
	void Swap(Glyph* pGlyph, const float _x, const float _y);
	
	Glyph* pGlyph;
	Matrix* poMatrix_uv;
	Matrix* poMatrix_orig;
};

#endif

//--- End of File -------------------------------------------------------------


