//----------------------------------------------------------------------------
// Copyright Ed Keenan 2019
//----------------------------------------------------------------------------

#include <assert.h>

#include "ShaderObject.h"
#include "GraphicsObject_FlatTexture.h"
#include "Texture.h"
#include "Model.h"

#include "CameraMan.h"
#include "CameraNode.h"
#include "Camera.h"


GraphicsObject_FlatTexture::GraphicsObject_FlatTexture(const Model * const _pModel, const ShaderObject * const _pShaderObj, Texture::Name _name)
	: GraphicsObject(_pModel, _pShaderObj), textName(_name)
{
	assert(pModel);
	assert(pShaderObj);
	assert(poWorld);
}

void GraphicsObject_FlatTexture::SetState()
{
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture
	GLuint textureID = Texture::Find(this->textName);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

void GraphicsObject_FlatTexture::SetDataGPU()
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
	glUniformMatrix4fv(this->pShaderObj->GetLocation("world_matrix"), 1, GL_FALSE, (float *)&world);;

}

void GraphicsObject_FlatTexture::Draw()
{
	// draw
	//glDrawArrays(GL_TRIANGLES, 0, (3 * this->GetModel()->numTris));

	//The starting point of the IBO
	glDrawElements(GL_TRIANGLES, 3 * this->GetModel()->numTris, GL_UNSIGNED_INT, 0);

}

void GraphicsObject_FlatTexture::RestoreState()
{
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
}

//--- End of File -------------------------------------------------------------

