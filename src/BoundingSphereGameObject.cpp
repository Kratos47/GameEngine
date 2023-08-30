//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "CameraMan.h"
#include "BoundingSphereGameObject.h"
#include "GraphicsObject_WireframeConstantColor.h"
#include "GameObject.h"
#include "GameTime.h"

BoundingSphereGameObject::BoundingSphereGameObject(GraphicsObject* pGraphicsObject)
	: GameObject(pGraphicsObject)
{
	this->pRefGameObj = nullptr;
	this->pScale = new Vect();
	this->pTrans = new Vect();
}

BoundingSphereGameObject::~BoundingSphereGameObject()
{
	delete this->pScale;
	delete this->pTrans;
}

void BoundingSphereGameObject::SetBoundingSphereReference(GameObject* pGameObj)
{
	this->pRefGameObj = pGameObj;
}

void BoundingSphereGameObject::privUpdate(Time currentTime)
{
	UNUSED_VAR(currentTime);

	// Goal: update the world matrix

	Matrix Scale(Matrix::Scale::XYZ, *this->pScale);
	Matrix TransA(Matrix::Trans::XYZ, *this->pTrans);

	// world matrix
	*this->poWorld = Scale * TransA;



	// Flip the color based on camera_0
	Camera* pCam = CameraMan::Find(Camera::Name::CAMERA_0);
	Sphere* pCurrSphere = this->poGraphicsObject->pCurrSphere;

	// Architecture issue... this doesn't feel good.
	GraphicsObject_WireframeConstantColor* pGObj = (GraphicsObject_WireframeConstantColor*)this->poGraphicsObject;
	if (pCam->CullTest(*pCurrSphere) == Camera::CullResult::CULL_OUTSIDE)
	{
		pGObj->pWireColor->set(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		pGObj->pWireColor->set(0.0f, 1.0f, 0.0f, 1.0f);
	}


}

void BoundingSphereGameObject::privUpdateBoundingSphereReference(Time currentTime)
{
	// incase the reference object wasn't updated first
	// update it now.

	this->pRefGameObj->Update(currentTime);

	// set the center and scale of bound box based on ref game object's world coord
	Sphere* pRefCurrSphere = this->pRefGameObj->GetGraphicsObject()->pCurrSphere;
	this->pScale->set(pRefCurrSphere->rad, pRefCurrSphere->rad, pRefCurrSphere->rad);
	this->pTrans->set(pRefCurrSphere->cntr);

	Sphere* pCurrSphere = this->GetGraphicsObject()->pCurrSphere;
	*pCurrSphere = *pRefCurrSphere;
}


void BoundingSphereGameObject::Update(Time currentTime)
{
	// Set Translation & Scale from reference
	this->privUpdateBoundingSphereReference(currentTime);

	// Goal: update the world matrix
	this->privUpdate(currentTime);

	// push to graphics object
	this->poGraphicsObject->SetWorld(*this->poWorld);
}

// End of File

