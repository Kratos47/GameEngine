//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#include <assert.h>

#include "GraphicsObject.h"
#include "MathEngine.h"
#include "Model.h"

GraphicsObject::GraphicsObject(const Model* const _pModel, const ShaderObject* const _pShaderObj)
	: pModel(_pModel),
	pShaderObj(_pShaderObj)
{
	this->poWorld = new Matrix(Matrix::Special::Identity);
	this->pCurrSphere = new Sphere();
	assert(_pModel);
	assert(_pShaderObj);
	assert(this->poWorld);
	assert(this->pCurrSphere);
}

GraphicsObject::~GraphicsObject()
{
	delete this->poWorld;
	delete this->pCurrSphere;
}

void GraphicsObject::Render()
{
	this->SetState();
	this->SetDataGPU();
	this->Draw();
	this->RestoreState();
}

const Model* GraphicsObject::GetModel() const
{
	return this->pModel;
}

Matrix& GraphicsObject::GetWorld()
{
	return *this->poWorld;
}

void GraphicsObject::SetWorld(Matrix& _world)
{
	*this->poWorld = _world;
}


//--- End of File -------------------------------------------------------------
