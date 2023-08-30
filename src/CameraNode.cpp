//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include <assert.h>

#include "CameraNode.h"
#include "Camera.h"

CameraNode::CameraNode( )
{
	this->poCamera = nullptr;
}

CameraNode::~CameraNode() 
{
	delete this->poCamera;
}

void CameraNode::set( Camera *camera )
{
	assert(camera);
	this->poCamera = camera;
}

//--- End of File -------------------------------------------------------------
