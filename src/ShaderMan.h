//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include "ShaderObject.h"

// Singleton
class ShaderMan
{
public:
	static void Add(ShaderObject *pShader);
	static void Create();
	static void Destroy();

	ShaderObject* Find(ShaderObject::Name reqName);

public:  // methods

	static ShaderMan *privGetInstance();
	ShaderMan();

	void privAddToFront(ShaderObject *node, ShaderObject *&head);
	void privRemove(ShaderObject *node, ShaderObject *&head);

private:  // add

	ShaderObject *active;

};

#endif

//--- End of File -------------------------------------------------------------

