//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef MODEL_MANAGER_H
#define MODEL_MANAGER_H

#include "Model.h"

// Singleton
class ModelMan
{
public:
	static void Add(Model *pModel);
	static void Create();
	static void Destroy();

private:  // methods

	static ModelMan *privGetInstance();
	ModelMan();

	void privAddToFront(Model *node, Model *&head);
	void privRemove(Model *node, Model *&head);

private:  // add

	Model *active;

};

#endif

//--- End of File -------------------------------------------------------------

