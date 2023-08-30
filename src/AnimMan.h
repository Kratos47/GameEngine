//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_MANAGER_H
#define ANIM_MANAGER_H


#include "AnimController.h"
// Singleton
class AnimMan
{
public:
	static void Add(AnimController* pClip);
	static void Create();
	static void Destroy();

public:  // methods

	static AnimMan* privGetInstance();
	static AnimController* Find(AnimController::InstancenName name);
	AnimMan();

	void privAddToFront(AnimController* node, AnimController*& head);

public:  // add

	AnimController* active;
};

#endif

//--- End of File -------------------------------------------------------------


