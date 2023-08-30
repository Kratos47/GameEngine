//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#include <assert.h>
#include "MathEngine.h"
#include "Model.h"
#include "ShaderObject.h"
#include "GraphicsObject_SkinColor.h"

#include "CameraMan.h"
#include "CameraNode.h"
#include "Camera.h"

//#include "BoneWeight.h"
//#include "eat.h"



GraphicsObject_SkinColor::GraphicsObject_SkinColor(const Model* const pModel, const ShaderObject* const pShaderObj, const unsigned int NumBones,
	AnimController::InstancenName name)
	: GraphicsObject(pModel, pShaderObj)
{
	assert(pModel);
	assert(pShaderObj);
	assert(poWorld);
	this->numBones = NumBones;
	this->instanceName = name;
}


void GraphicsObject_SkinColor::SetState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void GraphicsObject_SkinColor::SetDataGPU()
{
	// use this shader
	this->pShaderObj->SetActive();

	// set the vao
	glBindVertexArray(this->GetModel()->vao);
	
	// Get active camera
	Camera* pCamera = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);

	//Matrix world = this->GetWorld();
	Matrix view = pCamera->getViewMatrix();
	Matrix proj = pCamera->getProjMatrix();

	glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float*)&proj);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float*)&view);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("invM"), numBones, GL_FALSE, (float*)InvBonePose);

	AnimController* anim = AnimMan::Find(instanceName);

	glUniformMatrix4fv(this->pShaderObj->GetLocation("boneWorld"), numBones, GL_FALSE, (float*)anim->currPose);
}

void GraphicsObject_SkinColor::Draw()
{
	// draw
	//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numVerts));

	//The starting point of the IBO
	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObject_SkinColor::RestoreState()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}


//--- End of File -------------------------------------------------------------
