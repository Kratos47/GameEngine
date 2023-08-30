//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_WIREFRAME_CONSTANT_COLOR_H
#define GRAPHICS_OBJECT_WIREFRAME_CONSTANT_COLOR_H

#include "GraphicsObject.h"

class GraphicsObject_WireframeConstantColor :public GraphicsObject
{
public:
	GraphicsObject_WireframeConstantColor() = delete;
	GraphicsObject_WireframeConstantColor(const GraphicsObject_WireframeConstantColor&) = delete;
	GraphicsObject_WireframeConstantColor& operator = (const GraphicsObject_WireframeConstantColor&) = delete;

	GraphicsObject_WireframeConstantColor(Model *model, ShaderObject *pShaderObj, Vect &wireColor);

	~GraphicsObject_WireframeConstantColor();

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;	
	virtual void Draw() override;
	virtual void RestoreState() override;

public:
// data:  place uniform instancing here
	Vect *pWireColor;
	
};
#endif

//--- End of File -------------------------------------------------------------
