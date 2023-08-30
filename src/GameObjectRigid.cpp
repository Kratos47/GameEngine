//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "MathEngine.h"
#include "GameObjectRigid.h"
#include "GraphicsObject.h"

#include "GameTime.h"

GameObjectRigid::GameObjectRigid(GraphicsObject  *pGraphicsObject)
	: GameObject(pGraphicsObject)
{
	assert(pGraphicsObject != 0);

	this->pScale = new Vect(1.0f, 1.0f, 1.0f);
	assert(this->pScale);

	this->pPos = new Vect(0.0f, 0.0f, 0.0f);
	assert(this->pPos);

	this->angle = 0.0f;
}

GameObjectRigid::~GameObjectRigid()
{
	delete this->pPos;
	delete this->pScale;
}

void GameObjectRigid::privUpdate(Time )
{

	/////CODE IN THE UPDATE
	//Matrix Scale(Matrix::Scale::XYZ, *this->pScale);
	//Matrix TransA(Matrix::Trans::XYZ, *this->pPos);
	//Matrix RotY(Matrix::Rot1::Y, this->angle);

	//// world matrix
	//*this->poWorld = Scale * RotY * TransA;
}

void GameObjectRigid::Update(Time currentTime)
{
	// Goal: update the world matrix
	this->privUpdate(currentTime);

	// update the bounding volume based on world matrix
	this->baseUpdateBoundingSphere();

	// push to graphics object
	this->poGraphicsObject->SetWorld(*this->poWorld);
}

void GameObjectRigid::SetPos(const Vect& pos)
{
	*this->pPos = pos;
}

void GameObjectRigid::SetScale(const Vect& scale)
{
	// deep copy
	*this->pScale = scale;
}
void GameObjectRigid::SetDeltaRot(float delta)
{
	this->angle = delta;
}

//--- End of File ------------------------------------------------------------
