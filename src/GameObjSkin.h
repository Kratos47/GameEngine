#ifndef GAME_OBJECT_SKIN_H
#define GAME_OBJECT_SKIN_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject.h"

class AnimController;

// Keenan(33)
class GameObjectSkin : public GameObject
{
public:
	GameObjectSkin(GraphicsObject* graphicsObject);

	// Big four
	GameObjectSkin() = delete;
	GameObjectSkin(const GameObjectSkin&) = delete;
	GameObjectSkin& operator=(GameObjectSkin&) = delete;
	virtual ~GameObjectSkin() override;

	virtual void Update(Time currentTime);

	//void SetPos(const Vect& pos);

	//void SetScale(const Vect& scale);

	//void SetDeltaRot(float delta);

public:
	void  GameObjectSkinUpdate(Time currentTime, AnimController* anim);

public:
	/*Vect* pScale;
	Vect* pPos;
	float angle;*/

};


#endif

//--- End of File ------------------------------------------------------------
