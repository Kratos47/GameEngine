//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#include <assert.h>
#include "MathEngine.h"
#include "Model.h"
#include "ShaderObject.h"
#include "GraphicsObject_SkinLight.h"

#include "CameraMan.h"
#include "CameraNode.h"
#include "Camera.h"
#include "SSBO_Man.h"

//#include "BoneWeight.h"
//#include "eat.h"



GraphicsObject_SkinLight::GraphicsObject_SkinLight(const Model* const pModel, const ShaderObject* const pShaderObj, const unsigned int NumBones,
	AnimController::InstancenName name)
	: GraphicsObject(pModel, pShaderObj)
{
	assert(pModel);
	assert(pShaderObj);
	assert(poWorld);
	this->numBones = NumBones;
	this->instanceName = name;
	this->poLightColor = new Vect(0.4, 0.4, 0);
	this->poLightPos = new Vect(10, 10, 10);
}


GraphicsObject_SkinLight::~GraphicsObject_SkinLight()
{
	delete this->poLightColor;
	delete this->poLightPos;
}


void GraphicsObject_SkinLight::SetState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void GraphicsObject_SkinLight::SetDataGPU()
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
	//glUniformMatrix4fv(this->pShaderObj->GetLocation("boneWorld"), numBones, GL_FALSE, (float*)anim->currPose);
	
	if (instanceName == AnimController::InstancenName::TEDDY_01) 
	{
		SSBOMan* instSSBO = SSBOMan::privGetInstance();

		SSBO* pSBO_World_Result = instSSBO->Find(SSBO::Name::TEDDY_WORLD);

		pSBO_World_Result->Bind(5);
	}
	if (instanceName == AnimController::InstancenName::HUMAN_01) 
	{
		SSBOMan* instSSBO = SSBOMan::privGetInstance();

		SSBO* pSBO_World_Result = instSSBO->Find(SSBO::Name::HUMAN_WORLD);

		pSBO_World_Result->Bind(5);
	}
	

	Vect light = *this->poLightPos;
	Vect color = *this->poLightColor;
	glUniform4fv(this->pShaderObj->GetLocation("vLightPos"), 1, (float*)&light);
	glUniform4fv(this->pShaderObj->GetLocation("vColor"), 1, (float*)&color);

	
}

void GraphicsObject_SkinLight::Draw()
{
	// draw
	//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numVerts));

	//The starting point of the IBO
	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObject_SkinLight::RestoreState()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}


//--- End of File -------------------------------------------------------------
