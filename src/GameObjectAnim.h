//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef ANIM_GAME_OBJECT_H
#define ANIM_GAME_OBJECT_H

#include "GameObject.h"
#include "MathEngine.h"
#include "ShaderObject.h"
#include "Model.h"
#include "GraphicsObject.h"
#include "PCSNode.h"
#include "SSBO.h"
class AnimController;

// Keenan(32)
class GameObjectAnim: public GameObject
{
public:
	GameObjectAnim(GraphicsObject  *graphicsObject);

	// Big four
	GameObjectAnim() = delete;
	GameObjectAnim(const GameObjectAnim &) = delete;
	GameObjectAnim &operator=(GameObjectAnim & tmp ) = delete;
	virtual ~GameObjectAnim() override;

	virtual void Update(Time currentTime);
	void AnimUpdate(Time currentTime, AnimController* cntrlr, float animSpeed, SSBO::Name name);
	

	Matrix GetBoneOrientation(void) const;
	void SetBoneOrientation(const Matrix &);
	void SetIndex(int val);

private:
	void privUpdate(AnimController* cntrlr,int boneIndx,SSBO::Name name);

public:
	Vect *pScale;
	Vect *pPos;
	Vect *pDof;
	Vect *pUp;

	Matrix      *pLocal;
	Matrix		*pBoneOrientation;
	int         indexBoneArray;

};


#endif

//--- End of File ------------------------------------------------------------
