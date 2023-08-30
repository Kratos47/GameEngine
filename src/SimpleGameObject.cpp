//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "MathEngine.h"
#include "SimpleGameObject.h"
#include "GameObject.h"
#include "GraphicsObject.h"
#include "Model.h"


SimpleGameObject::SimpleGameObject(GraphicsObject* pGraphicsObject)
	: GameObject(pGraphicsObject)
{
	assert(pGraphicsObject != 0);
	this->pTrans = new Vect(0.0f, 0.0f, 0.0f);
	this->pScale = new Vect(1.0f, 1.0f, 1.0f);
	assert(pTrans);
	assert(pScale);
	assert(pGraphicsObject != 0);

	this->rotY = 0.0f;
	this->rotZ = 0.0f;
	this->deltaRotZ = 0.0f;
	this->deltaRotY = 0.0f;
}

SimpleGameObject::~SimpleGameObject()
{
	delete this->pTrans;
	delete this->pScale;
}

void SimpleGameObject::SetMove(MoveDir move)
{
	this->moveDir = move;
};
void SimpleGameObject::privUpdate(Time currentTime )
{
	//// Goal: update the world matrix

	this->rotY += this->deltaRot;
	this->rotZ += this->deltaRot;

	Matrix Scale(Matrix::Scale::XYZ, *this->pScale);
	Matrix TransA(Matrix::Trans::XYZ, *this->pTrans);
	Matrix RotY(Matrix::Rot1::Y, this->rotY);
	Matrix RotZ(Matrix::Rot1::Z, this->rotZ);
	
	// world matrix
	*this->poWorld = Scale * RotZ * RotY *TransA;
	
}


void SimpleGameObject::Update(Time currentTime)
{
	// Goal: update the world matrix
	this->privUpdate(currentTime);

	// update the bounding volume based on world matrix
	this->baseUpdateBoundingSphere();

	// push to graphics object
	this->poGraphicsObject->SetWorld(*this->GetWorld());
}

void SimpleGameObject::SetPos(const Vect& pos)
{
	*this->pTrans = pos;
}

void SimpleGameObject::SetScale(const Vect& scale)
{
	// deep copy
	*this->pScale = scale;
}
void SimpleGameObject::SetDeltaRot(float delta)
{
	this->deltaRot = delta;
}


//--- End of File ------------------------------------------------------------