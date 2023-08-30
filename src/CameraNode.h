//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CAMERA_NODE_H
#define CAMERA_NODE_H

class Camera;

class CameraLink
{
public:
	CameraLink()
	{
		this->next = 0;
		this->prev = 0;
	}

	virtual ~CameraLink()
	{

	}

	CameraLink *next;
	CameraLink *prev;
};

class CameraNode: public CameraLink
{
public:
	CameraNode();
	virtual ~CameraNode();

	void set( Camera *pCamera);

public:
	Camera *poCamera;

};

#endif

//--- End of File -------------------------------------------------------------
