//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H
//
#include "MathEngine.h"

#include "ShaderObject.h"
#include "RenderMaterial.h"

class Matrix;
class ShaderObject;
class Model;
class Sphere;

class GraphicsObject : private RenderMaterial
{
public:
	GraphicsObject(const Model* const pModel, const ShaderObject* const pShaderObj);

	GraphicsObject() = delete;
	GraphicsObject(const GraphicsObject&) = delete;
	GraphicsObject& operator = (const GraphicsObject&) = delete;
	virtual ~GraphicsObject();

	void Render();
	void SetWorld(Matrix& _world);

	const Model* GetModel() const;
	Matrix& GetWorld();

	Sphere* pCurrSphere;

protected:
	const Model         *pModel;
	Matrix              *poWorld;
	const ShaderObject  *pShaderObj;
};

#endif

//--- End of File -------------------------------------------------------------

