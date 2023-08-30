//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_NULL_H
#define GRAPHICS_OBJECT_NULL_H

#include "GraphicsObject.h"

class GraphicsObject_Null :public GraphicsObject
{
public:
	GraphicsObject_Null(const Model * const pModel, const ShaderObject * const pShaderObj);
	GraphicsObject_Null() = delete;
	GraphicsObject_Null(const GraphicsObject_Null &) = delete;
	GraphicsObject_Null & operator = (const GraphicsObject_Null &) = delete;
	~GraphicsObject_Null() = default;

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// data:  place uniform instancing here

};

#endif

//--- End of File -------------------------------------------------------------


