//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_SKIN_LIGHT_H
#define GRAPHICS_OBJECT_SKIN_LIGHT_H

#include "GraphicsObject.h"
#include "AnimController.h" 
#include "AnimMan.h"
class GraphicsObject_SkinLight :public GraphicsObject
{
public:

	GraphicsObject_SkinLight(const Model* const pModel, const ShaderObject* const pShaderObj, const unsigned int NumBones,
		AnimController::InstancenName name);
	GraphicsObject_SkinLight() = delete;
	GraphicsObject_SkinLight(const GraphicsObject_SkinLight&) = delete;
	GraphicsObject_SkinLight& operator = (const GraphicsObject_SkinLight&) = delete;
	~GraphicsObject_SkinLight();

	// Rendermaterial contract
	virtual void SetState() override;
	virtual void SetDataGPU() override;
	virtual void Draw() override;
	virtual void RestoreState() override;

	// data:  place uniform instancing here
	Matrix* InvBonePose;
	Vect* poLightColor;
	Vect* poLightPos;
	int numBones;
	AnimController::InstancenName instanceName;
};
#endif


//--- End of File -------------------------------------------------------------
