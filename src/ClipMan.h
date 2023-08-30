//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef CLIP_MANAGER_H
#define CLIP_MANAGER_H

//#include "Anim.h"
#include "FrameBucket.h"
#include "Clip.h"
// Singleton
class ClipMan
{
public:
	static void Add(Clip* pClip);
	static void Create();
	static void Destroy();

public:  // methods

	static ClipMan* privGetInstance();
	static Clip* Find(Clip::ClipName name);
	ClipMan();

	void privAddToFront(Clip* node, Clip*& head);
	void privRemove(Clip* node, Clip*& head);

public:  // add

	Clip* active;
};

#endif

//--- End of File -------------------------------------------------------------


