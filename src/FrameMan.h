//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef FRAME_MANAGER_H
#define FRAME_MANAGER_H

//#include "Anim.h"
#include "FrameBucket.h"

// Singleton
class FrameMan
{
public:
	static void Add(Frame_Bucket* pShader);
	static void Create();
	static void Destroy();

public:  // methods

	static FrameMan* privGetInstance();
	FrameMan();

	void privAddToFront(Frame_Bucket* node, Frame_Bucket*& head);
	void privRemove(Frame_Bucket* node, Frame_Bucket*& head);

public:  // add

	Frame_Bucket* active;

	///Dangling data for now 
	//unsigned int numBones;
};

#endif

//--- End of File -------------------------------------------------------------


