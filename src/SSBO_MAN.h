//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SSBO_MANAGER_H
#define SSBO_MANAGER_H

#include "SSBO.h"
// Singleton
class SSBOMan
{
public:
	static void Add(SSBO* pSSBO);
	static void Create();
	static void Destroy();

	SSBO* Find(SSBO::Name reqName);

public:  // methods

	static SSBOMan* privGetInstance();
	SSBOMan();

	void privAddToFront(SSBO* node, SSBO*& head);
	void privRemove(SSBO* node, SSBO*& head);

public:  // add

	SSBO* active;

};

#endif

//--- End of File -------------------------------------------------------------

