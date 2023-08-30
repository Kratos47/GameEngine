//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#include <assert.h>
#include "MathEngine.h"
#include "Model.h"
#include "ShaderObject.h"
#include "GraphicsObject_ColorByPosition.h"

#include "CameraMan.h"
#include "CameraNode.h"
#include "Camera.h"

GraphicsObject_ColorByPosition::GraphicsObject_ColorByPosition(const Model * const pModel, const ShaderObject * const pShaderObj)
: GraphicsObject(pModel, pShaderObj)
{
	assert(pModel);
	assert(pShaderObj);
	assert(poWorld);
}

void GraphicsObject_ColorByPosition::SetState()
{
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
}

void GraphicsObject_ColorByPosition::SetDataGPU()
{
	// use this shader
	this->pShaderObj->SetActive();	
	
	// set the vao
	glBindVertexArray(this->GetModel()->vao);
	
	// Get active camera
	Camera *pCamera = CameraMan::GetCurrent(Camera::Type::PERSPECTIVE_3D);

	Matrix world = this->GetWorld();
	Matrix view = pCamera->getViewMatrix();
	Matrix proj = pCamera->getProjMatrix();

	glUniformMatrix4fv(this->pShaderObj->GetLocation("proj_matrix"), 1, GL_FALSE, (float *)&proj);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("view_matrix"), 1, GL_FALSE, (float *)&view);
	glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float *)&world);

}

void GraphicsObject_ColorByPosition::Draw()
{	     
	// draw
	//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numVerts));

	//The starting point of the IBO
	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);
}

void GraphicsObject_ColorByPosition::RestoreState()
{	
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
}


//--- End of File -------------------------------------------------------------
