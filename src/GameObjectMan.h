//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_MAN_H
#define GAME_OBJECT_MAN_H

#include "GameObject.h"
#include "PCSTree.h"
class AnimController;

// Singletonv
class GameObjectMan
{
public:
	static void Add(GameObject* pObj, GameObject* pParent);
	static void Add( GameObject *pObj);
	static void Draw( void );
	static void Update(Time t, char* gamObj);
	static void Update(Time currentTime, AnimController* cntrlr, float animSpeed);
	static GameObject *GetRoot(void);
	static void Destroy();
	static void Update(Time currentTime);
	~GameObjectMan();
	static PCSTree *GetPCSTree();
public:
	GameObjectMan();


	static GameObjectMan *privGetInstance();

// data
	PCSTree *poRootTree;
};

#endif

//--- End of File ------------------------------------------------------------
