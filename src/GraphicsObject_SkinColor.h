//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_SKIN_COLOR_H
#define GRAPHICS_OBJECT_SKIN_COLOR_H

#include "GraphicsObject.h"
#include "AnimController.h" 
#include "AnimMan.h"
class GraphicsObject_SkinColor :public GraphicsObject
{
public:
	
	GraphicsObject_SkinColor(const Model* const pModel, const ShaderObject* const pShaderObj, const unsigned int NumBones,
		AnimController::InstancenName name);
	GraphicsObject_SkinColor() = delete;
	GraphicsObject_SkinColor(const GraphicsObject_SkinColor&) = delete;
	GraphicsObject_SkinColor& operator = (const GraphicsObject_SkinColor&) = delete;
	~GraphicsObject_SkinColor() = default;

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// data:  place uniform instancing here
	Matrix* InvBonePose;
	AnimController::InstancenName instanceName;
	int numBones;
};
#endif


//--- End of File -------------------------------------------------------------
