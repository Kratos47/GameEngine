//----------------------------------------------------------------------------
// Copyright 2019, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "Game.h"
#include "MathEngine.h"
#include "GameObjectAnim.h"
#include "GraphicsObject.h"
#include "Model.h"
#include "FrameBucket.h"
#include "FrameMan.h"
#include "ClipMan.h"
#include "GameObjectMan.h"
#include "AnimController.h"
#include "PCSTreeForwardIterator.h"
#include "ShaderMan.h"
#include "SSBO_Man.h"
#include "Framework.h"

GameObjectAnim::GameObjectAnim(GraphicsObject  *pGraphicsObject)
	: GameObject(pGraphicsObject)
{
	assert(pGraphicsObject != 0);

	this->pDof = new Vect(0.0f, 0.0f, 0.0f);
	assert(pDof);

	this->pUp = new Vect(0.0f, 1.0f, 0.0f);
	assert(pUp);

	this->pScale = new Vect(1.0f, 1.0f, 1.0f);
	assert(pScale);

	this->pPos = new Vect(0.0f, 0.0f, 0.0f);
	assert(pPos);

	//this->pBoneParent = nullptr;

	this->indexBoneArray = 0;

	this->pLocal = new Matrix(Matrix::Special::Identity);
	assert(pLocal);

	this->pBoneOrientation = new Matrix(Matrix::Special::Identity);
	assert(pBoneOrientation);

}



GameObjectAnim::~GameObjectAnim()
{
	delete this->pDof;
	delete this->pUp;
	delete this->pPos;
	delete this->pScale;
	delete this->pLocal;
	delete this->pBoneOrientation;
}

void GameObjectAnim::Update(Time t)
{
}

void GameObjectAnim::AnimUpdate(Time currentTime, AnimController* cntrlr, 
	float animSpeed, SSBO::Name name)
{
	ShaderMan* inst = ShaderMan::privGetInstance();

	ShaderObject* pComputeShader = inst->Find(ShaderObject::Name::COMPUTE_WORLD);
	pComputeShader->SetActive();

	// Now bind the data (sbo's)
	// Since it is already on GPU... we can now just bind it

	SSBOMan* instSSBO = SSBOMan::privGetInstance();

	SSBO* pSBO_World_Result = nullptr;

	SSBO* pSBO_Local = nullptr;

	int numBones = cntrlr->currAnimBones;

	//detrmine dispatch count
	if (name == SSBO::Name::TEDDY_IDLE_RESULT || name == SSBO::Name::TEDDY_WALK_RESULT)
	{
		if (name == SSBO::Name::TEDDY_IDLE_RESULT)
		{
			pSBO_Local = instSSBO->Find(SSBO::Name::TEDDY_IDLE_RESULT);
			pSBO_Local->Bind(0);
		}
		if (name == SSBO::Name::TEDDY_WALK_RESULT)
		{
			pSBO_Local = instSSBO->Find(SSBO::Name::TEDDY_WALK_RESULT);
			pSBO_Local->Bind(0);
		}

		SSBO* pSBO_Hierarchy = instSSBO->Find(SSBO::Name::TEDDY_HIERARCHY);

		pSBO_Hierarchy->Bind(1);

		pSBO_World_Result = instSSBO->Find(SSBO::Name::TEDDY_WORLD);

		pSBO_World_Result->Bind(2);

		glUniform1i(pComputeShader->GetLocation("boneCount"), cntrlr->currAnimBones);
		glUniform1i(pComputeShader->GetLocation("hierarchyDepth"), cntrlr->hierarchyDeapth);

		// Special transform for Teddy
		Vect Tr = Vect(40, 0, 3000);
		Matrix Te = Matrix(Matrix::Trans::XYZ, Tr);
		Matrix Rx = Matrix(Matrix::Rot1::X, MATH_PI2);
		Matrix Rz = Matrix(Matrix::Rot1::Z, 1.7f);
		Matrix MT = Te * Rx * Rz;

		glUniformMatrix4fv(pComputeShader->GetLocation("parentWorld"), 1, GL_FALSE, (float*)&MT);

		// Kick it... start the compute shader
		pComputeShader->Dispatch(1, 1, 1);


	}
	else if (name == SSBO::Name::HUMAN_RUN_RESULT || name == SSBO::Name::HUMAN_WALK_RESULT)
	{
		if (name == SSBO::Name::HUMAN_RUN_RESULT)
		{
			pSBO_Local = instSSBO->Find(SSBO::Name::HUMAN_RUN_RESULT);
			pSBO_Local->Bind(0);
		}
		if (name == SSBO::Name::HUMAN_WALK_RESULT)
		{
			pSBO_Local = instSSBO->Find(SSBO::Name::HUMAN_WALK_RESULT);
			pSBO_Local->Bind(0);
		}

		SSBO* pSBO_Hierarchy = instSSBO->Find(SSBO::Name::HUMAN_HIERARCHY);

		pSBO_Hierarchy->Bind(1);

		pSBO_World_Result = instSSBO->Find(SSBO::Name::HUMAN_WORLD);

		pSBO_World_Result->Bind(2);

		glUniform1i(pComputeShader->GetLocation("boneCount"), cntrlr->currAnimBones);
		glUniform1i(pComputeShader->GetLocation("hierarchyDepth"), cntrlr->hierarchyDeapth);

		Vect Trans = Vect(-680.172638f, 1288.917847f - 500, 526.212402f);
		Matrix T = Matrix(Matrix::Trans::XYZ, -Trans);
		Matrix M = T;

		Matrix R = Matrix(Matrix::Rot1::Z, 1.5f);

		M = M * R;
	

		glUniformMatrix4fv(pComputeShader->GetLocation("parentWorld"), 1, GL_FALSE, (float*)&M);

		// Kick it... start the compute shader
		pComputeShader->Dispatch(4, 1, 1);
	}

	
	// -----------------------------------------------
	// update the time
	// ----------------------------------------------

	ClipMan* pClipMan = ClipMan::privGetInstance();
	assert(pClipMan);

	Clip* pActiveClip = pClipMan->active;

	Time deltaTime = animSpeed * Time(Time::NTSC_30_FRAME);

	cntrlr->tCurrent += deltaTime;


	// protection for time values for looping
	if (cntrlr->tCurrent < Time(Time::ZERO))
	{
		cntrlr->tCurrent = pActiveClip->maxTime;
	}
	else if (cntrlr->tCurrent > pActiveClip->maxTime)
	{
		cntrlr->tCurrent = Time(Time::ZERO);
	}
	else
	{
		// do nothing
	}	

	// Block Waiting for stages to complete
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void GameObjectAnim::SetIndex(int val)
{
	this->indexBoneArray = val;
}

void GameObjectAnim::SetBoneOrientation(const Matrix &tmp)
{
	*this->pBoneOrientation = tmp;
}

Matrix GameObjectAnim::GetBoneOrientation(void) const
{
	return Matrix(*this->pBoneOrientation);
}


//--- End of File ------------------------------------------------------------
