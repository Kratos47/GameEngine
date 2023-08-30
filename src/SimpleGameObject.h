//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef SIMPLE_GAME_OBJECT_H
#define SIMPLE_GAME_OBJECT_H

#include "GameObject.h"
#include "MathEngine.h"
#include "ShaderObject.h"
#include "Model.h"
#include "GraphicsObject.h"
#include "PCSNode.h"

class SimpleGameObject : public GameObject
{
public:
	enum MoveDir
	{
		Move_X,
		Move_Y,
		Move_Z,
		Move_None
	};

public:
	SimpleGameObject(GraphicsObject* graphicsObject);

	// Big four
	SimpleGameObject() = delete;
	SimpleGameObject(const SimpleGameObject&) = delete;
	SimpleGameObject& operator=(SimpleGameObject&) = delete;
	virtual ~SimpleGameObject() override;

	virtual void Update(Time currentTime);

	void SetPos(const Vect& pos);
	void SetScale(const Vect& scale);
	void SetDeltaRot(float delta);

	void SetMove(MoveDir move);
private:
	void privUpdate(Time currentTime);

public:   // add accessors later
	Vect* pScale = 0;
	Vect* pTrans = 0;

	float deltaRot = 0;
	float deltaRotY = 0;
	float deltaRotZ = 0;
	float rotY = 0;
	float rotZ = 0;
	MoveDir moveDir;
};


#endif

//--- End of File ------------------------------------------------------------
