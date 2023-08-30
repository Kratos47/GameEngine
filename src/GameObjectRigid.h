//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GAME_OBJECT_RIGID_H
#define GAME_OBJECT_RIGID_H

#include "GameObject.h"
#include "MathEngine.h"
#include "GraphicsObject.h"

class Time;
// Keenan(33)
class GameObjectRigid : public GameObject
{
public:
	GameObjectRigid(GraphicsObject  *graphicsObject);

	// Big four
	GameObjectRigid() = delete;
	GameObjectRigid(const GameObjectRigid &) = delete;
	GameObjectRigid &operator=(GameObjectRigid &) = delete;
	virtual ~GameObjectRigid() override;

	virtual void Update(Time currentTime);

	void SetPos(const Vect& pos);

	void SetScale(const Vect& scale);

	void SetDeltaRot(float delta);

public:
	void privUpdate(Time currentTime);

public:
	Vect *pScale;
	Vect *pPos;
	float angle;

};


#endif

//--- End of File ------------------------------------------------------------
