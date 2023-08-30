#include "GameObjSkin.h"
#include"AnimController.h"
#include"GameObjectMan.h"
#include "PCSTreeForwardIterator.h"
GameObjectSkin::GameObjectSkin(GraphicsObject* graphicsObject)
	: GameObject(graphicsObject)
{
	assert(graphicsObject != 0);
}

GameObjectSkin::~GameObjectSkin()
{
}

void GameObjectSkin::Update(Time currentTime)
{
}

void GameObjectSkin::GameObjectSkinUpdate(Time currentTime, AnimController* anim)
{
	UNUSED_VAR(currentTime);
	UNUSED_VAR(anim);
}
